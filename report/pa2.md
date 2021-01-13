# PA2 简单复杂的机器: 冯诺依曼计算机系统

    **这一个篇章过于久远，所以我就回答我所做的必答题。大部分的内容在几个月前时我就完成了，具体可以查看commit log，所以重点不会放在阐述心得和体会，更多关注在必答题上。**

    PA1: 9月 - 10月
    PA2: 10月 - 11月
    PA3: 12月
    PA4: 12月 - 1月

    PA1-PA3的感受会在PA3的时候讲述。PA4因为我一直有一个bug调不出来（多道程序），所以我只完成了一半，真的非常非常遗憾！

## 2.1 请整理一条指令在NEMU中的执行过程

    - 1.cpu_exec() -> exec_once()
    
    - 2.isa_exec()

    - 3.opcode_table -> decode_helper & execute_helper

    - 4.update_pc()

## 2.2 编译与链接

    分别去掉static/inline 都没有关系，同时去掉就不行

    理由：inline是内联减小函数跳转的开销、static是只作用于本文件内。inline作用后的func是一个弱符号，相对于强符号来说并不会在链接期间产生影响。
  
## 2.3 在nemu/include/common.h中添加一行volatile static int dummy; 然后重新编译NEMU. 请问重新编译后的NEMU含有多少个dummy变量的实体? 你是如何得到这个结果的?

    - 34个， nm riscv32-nemu | grep -c "dummy"
    - 34个， 因为未初始化的全局变量是弱符号，弱符号只会选择一个
    - 会报链接重复定义错误，因为此时都是强符号。

## 2.4 Makefile

    Makefile的组织方式在nemu、nexus-am、nanoslite里面都有一些不太一样，但运行方式大同小异：确定ISA（ARCH）、添加相应的头文件、编译链接、生成可执行二进制文件。
