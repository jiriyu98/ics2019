# PA3 穿越时空的旅程: 批处理系统

    这一部分的PA3花了我很久很久的时间，PA3.1和PA3.2的Bug困扰了我很久。我记忆犹新的一个是文件的读写的部分一直出错，我查了好久没有查出来。进行了很久的尝试，整整2个晚上，超过20小时的时间耗在上面了。解决问题之后，没想到后续进行的非常顺利，因为上下文结构其实是比较简单的，我自认为自己对函数的调用和控制流的学习和掌握是非常好的，所以在这一部分上，很轻松的理清了控制流、数据流就完成了；最后运行仙剑奇侠传也是很顺理成章的事。
    说实话，在PA3之前，我以为PA应该比较简单，所以也没有很认真对待，都是直接拿gedit(ubuntu自带的编辑器)写的代码，进展的也比较顺利，每天花个一点时间也就把PA1-PA2做完了。但PA3的时候，我发现事情没有这么简单，他的控制流和数据流开始复杂了起来，一些编码的安排也需要我好好考虑。我不得不切换到了sublime/vscode这样的编辑器上，全局搜索和函数跳转变得非常重要了。同事在做PA3的时候，也正是我实习刚开始的阶段，那时候我在商汤实习了一个月，这次实习也是我第一次写Cpp的工程性的代码。想着写pa和实习能相辅相成，我开始了工作日白天实习、晚上写PA（的生活，持续了一周多，我终于完成了PA3。但其中的辛酸只有我自己知道，睡不饱的觉、做不完的工作、分给PA时间很少！高强度无休的生活让我身心巨疲，好在最终做完了它，而且元旦来的恰到好处也让我喘了一口气。

    我的感悟（PA1-PA3）：
    
    第一，任何时候不要偷懒、或者不要过度高估自己的能力。PA1和PA2我没有上vscode的原因也是因为我想锻炼一下自己心中解码的能力，但是那个时候我比较空，也没有开始实习，所以可以慢慢做、好好思考；但是PA3的时候确实复杂度升高、我自己也缺乏时间，所以必须得上vscode了。
    
    第二，基础设施很重要，尤其是工具性的，gdb、monitor等。
    
    第三、我在阿里实习的时候花很多时间研究过Flink的源代码，在商汤实习的时候也研究过Apollo的源代码。但我仍然会在面对一份心得源代码时候感到不适应和恐惧，做PA的时候就有这样的体会！我觉得好的写代码的方式应该让别人来给你讲应该是怎么样怎么样，而不是全程自己去悟，这样很浪费时间。我很想吐槽PA的一点就是，他作为一个教学的工具，一些地方（炫技、不清晰）是不够合理的，它适合有很多时间的学生、尤其是NJU大二的学生。但对于大四的学生它出现的不尴不尬，有好有坏。当然，我很佩服作者的编码能力，不过我也确实不是搞计算机系统方向的，所以我更多的是站在软件工程的角度上去审视它。:)

    PA1 - PA3也终于告一段落了，PA4的部分我会另开一部分写。我并没有完成PA4，在多道程序的上面我有bug，并且一周之内我还没有解掉，所以我也不准备再投入更多的时间了，毕竟马上ddl了，还有很多生活上、学业上、工作上的事等待着我去解决。但请相信我，在一段时间的休整后，我会打开PA重新开始我的PA4.

## 理解上下文结构体的前世今生

    trap.S 中含有一个__am_asm_trap函数。它是为了针对不同的指令集，上下文结构传递的操作不同而设计的。它实际上就是做了压栈和出栈这两个工作。scause、sstatus、sepc这三个寄存器的值因此被传递给了__am_irq_handle函数。__Context也就因此作为其函数参数获得了上下文结构。随后，从函数返回后，就进行了出栈操作。

## 穿越时空的旅程

    1 asm volatile("li a7, -1; ecall"); 这个是__yeild()做的事。可以发现，它调用了ecall指令。
    2 而ecall指令需要我们自己去实现，实现后，它做了这样的事：raise_intr(reg_l(17), decinfo.seq_pc - 4);参数为当前寄存器a7的值和当前pc的值。raise_intr就是触发了一个中断。
        decinfo.isa.sepc = epc;
        decinfo.isa.scause = NO;
        decinfo.jmp_pc = decinfo.isa.stvec;
        decinfo_set_jmp(true);
    3 上文中有一个stvec，这个就是我们即将跳转到的地址。但是它是在什么时候设置的呢？原来nanos-lite的main里面，有一个init_irq(): 
        void init_irq(void) {
        Log("Initializing interrupt/exception handler...");
        _cte_init(do_event);
        }
    哦～原来在这里调用了_cte_init，这是什么？让我们再进入其中看一下，
        asm volatile("csrw stvec, %0" : : "r"(__am_asm_trap));
    怎么样，这下一切都很清楚了。原来它是调用csrw指令，然后把__am_asm_trap的函数地址一直存在stvec中了。
    4 有了上文的分析，其实后续的事情都是水到渠成了，它调用了__am_asm_trp然后传递给__am_irq_handle，进行中断/系统调用。
    5 __am_irq_handle函数会接受所进来的各种中断信息，将它封装成一类系统调用事件，分发给user_handler进行处理。等一下，我们这里是不是提到了user_handler？他是什么？其实它只是一个参数，接受了do_event这个处理event的函数。
    6 do_event中我们又调用了do_syscall，它进入到do_syscall进行更细致的处理。停一下，那么这些syscall又是怎么细致地被用的呢？这个可不是能一笔带过的事。进入到navy-apps/libs/libos/src/nanos.c 哇哦～所有事情都不言而喻了。
    7 让我们回到这次旅程中，后续的步骤应该比较简单，就是恢复现场，我不再赘述。

## hello程序是什么, 它从而何来, 要到哪里去

    1 hello程序是什么？显然，它是一个程序。但是和我们平常的程序一样么？显然不是。它被首先编译成二进制可执行文件，放进了ramdisk里面。随后Makefile还让这些文件被引入了files.h里面。使我们可以直接通过ramdisk读取、执行的方式使用它。就好像一个真实的计算机文件系统一样，我们读取编译好的二进制文件然后执行它。
    2 其实细节比较简单，nanos-lite加载ramdisk，将hello放在了files.h里面的一个位置，记录下它的offset。loder就是加载，它读取hello的起始位置，解析elf信息，将所需要的程序执行位置复制到内存中，返回hello的入口。有了entry，也就有了第一条hello应该执行的指令。
    3 接下来，让我们把目光放在hello内部。它调用了write和printf。printf会去调用write，write最后会进入上述说的navy-apps/libs/libos/src/nanos.c里的_write中，也就是一次系统调用。它调用了sys_write进行输出。当然，sys_write是可以选择不同的输出端口的（我们之前的文件系统已经实现了这个功能！），printf指定了输出端口，也就是stdout，最终调用serial_write进行输出。我们实现了serial_write的功能，使用的就是_putc进行单个字符的输出。

## 仙剑奇侠传究竟如何运行

    1.main.c中，lpBitmapDown&Up(// Create the surfaces)
    2.调用PAL_MKFReadChunk()将mgo.mkf中的图像读取到bitmaps(// Read the bitmaps)
    3.后续调用：SDL_SetPalette -> redraw() -> NDL_DrawRect() -> NDL_Render()
        fbdev = fopen("/dev/fb", "w"); assert(fbdev);
        
        int NDL_Render() {
            if (has_nwm) {
                fflush(stdout);
            } else {
                for (int i = 0; i < canvas_h; i ++) {
                fseek(fbdev, ((i + pad_y) * screen_w + pad_x) * sizeof(uint32_t), SEEK_SET);
                fwrite(&canvas[i * canvas_w], sizeof(uint32_t), canvas_w, fbdev);
                }
                fflush(fbdev);
                putc(0, fbsyncdev);
                fflush(fbsyncdev);
            }
        }
    4. 再来看看init.c里面用到的NDL_Render(), 上面的代码是NDL_Render的内容。很明显，他是执行了输出到canvas的过程，也就是输出到/dev/fb上。这就简单了，他的write是fb_write，也就是调用了draw_rect进行图像输出。
