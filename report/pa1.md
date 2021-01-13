# PA1 开天辟地的篇章: 最简单的计算机

    **这一个篇章过于久远，所以我就回答我所做的必答题。大部分的内容在几个月前时我就完成了，具体可以查看commit log，所以重点不会放在阐述心得和体会，更多关注在必答题上。**

    PA1: 9月 - 10月
    PA2: 10月 - 11月
    PA3: 12月
    PA4: 12月 - 1月

    PA1-PA3的感受会在PA3的时候讲述。PA4因为我一直有一个bug调不出来（多道程序），所以我只完成了一半，真的非常非常遗憾！

## 1.1 我选择的ISA

    riscv32

## 1.2 理解基础设施

    - 总调试时间 500 * 90% * 30s * 20 = 75h
    - 节省的调试时间 75h * 2 / 3 = 50h
  
## 1.3 理解基础设施

    R reg - reg
    I Immediate value & Load
    S Store
    B branch
    U Long Immediate value
    J Jump

## 1.4 LUI指令行为

    高位立即数加载 (Load Upper Immediate). U-type, RV32I and RV64I.
    将符号位扩展的 20 位立即数 immediate 左移 12 位，并将低 12 位置零，写入 x[rd]中。

## 1.5 mstatus寄存器的结构是怎么样的?

    union{
        struct {
        uint32_t      : 1;
        uint32_t SIE  : 1;
        uint32_t      : 2;
        uint32_t UPIE : 1;
        uint32_t SPIE : 1;
        uint32_t      : 2;
        uint32_t SPP  : 1;
        uint32_t      : 4;
        uint32_t FS   : 2;
        uint32_t XS   : 2;
        uint32_t      : 1;
        uint32_t SUM  : 1;
        uint32_t MXR  : 1;
        uint32_t      :11;
        uint32_t SD   : 1;
        };
        uint32_t val;
    } sstatus;

## 1.6 Shell命令

### .c .h统计

    find . -name "*.[ch]" |xargs cat|wc -l

### 非空行

    find . -name "*.[ch]" |xargs cat|grep -v ^$|wc -l

## 1.7 gcc -Wall -Werror

- Wall

    This enables all the warnings about constructions that some users consider questionable, and that are easy to avoid (or modify to prevent
    the warning), even in conjunction with macros.  This also enables some
    language-specific warnings described in C++ Dialect Options and
    Objective-C and Objective-C++ Dialect Options.

- Werror

    Make all warnings into errors.

- why using them?

    大的工程都会开启这两个选项，为了更好的定位问题
