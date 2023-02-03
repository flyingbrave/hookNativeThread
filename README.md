# hookNativeThread
一个简单的hook Native thread 的例子

(1)新建一个so ,里面写入新建线程的代码,通过pthread_create创建，并输出对应的so库libhelloworld.so
(2) 将libhelloworld.so加载进项目，并编写hookThread的cpp代码
(3) 从/proc/self/maps拿到基地址，通过readif -r（mac上用greadelf -r）获取到这个so的偏移地址，通过基地址+偏移地址就能获取到这个函数的真实地址
   

###
greadelf -r /Users/brave/Desktop/My222/app/libs/arm64-v8a/libhelloworld.so
输出
Relocation section '.rela.plt' at offset 0xe358 contains 78 entries:
  Offset          Info           Type           Sym. Value    Sym. Name + Addend
000000034ca8  000300000402 R_AARCH64_JUMP_SL 0000000000000000 pthread_create@LIBC + 0
###

在.rela.plt中查到pthread_create的便宜地址为000000034ca8
（4）然后把拿到的真实地址指向我们的hook函数my_pthread_create,从而实现hook_native_thread的目的 


引申
上面我们hook的偏移都是基于通过readif看到的偏移地址，但是实际上这个地址都用readif可能会非常不方便，而且我们也只是检索了rel.plt表，实际上会存在多个复杂的跳转现象时，就需要检索所有的重定位表。但是没关系，这些xhook bhook都帮我们做了，只需要调用封装好的方法即可。


日志输出：
02-03 15:01:44.581 26041 26041 D tag5    : hookThread 6666  
02-03 15:01:44.583 26041 26041 D tag5    :  基地址 1286643712
02-03 15:01:44.583 26041 26041 D tag5    :  真实地址 1286859944
02-03 15:01:44.583 26041 26041 D tag5    :  添加完权限
02-03 15:01:44.583 26041 26041 D tag5    : 替换为函数地址  1286859944
02-03 15:01:44.583 26041 26041 D tag5    :  清除缓存
02-03 15:01:44.583 26041 26041 D tag5    : pthread hook power by brave
02-03 15:01:44.583 26041 26069 D tag5    :  pthread start!
02-03 15:01:44.583 26041 26069 D tag5    : member->num:10086
02-03 15:01:44.583 26041 26069 D tag5    : member->name:brave

“pthread hook power by brave”，这个日志的输出代表我们的hook成功，而“pthread start!”日志的输出代表线程真正的被执行了



