### 【2273】report part name 报告部件名称

#### 代码

```cpp
    #define RPN(X) (report_part_name(#X, X))  
    static void report_part_name(char *which, tag_t part)  
    {  
        char  
            fspec[MAX_FSPEC_SIZE+1];  
        UF_PART_ask_part_name(part, fspec);  
        ECHO("%s = %s\n", which, fspec);  
    }

```

#### 代码解析

> 这段代码是NX Open C++ API的二次开发示例，实现了以下功能：
>
> 1. 定义了一个宏RPN(X)，用于输出NX部件的名称。它接收一个部件标识符X，并将其名称输出到控制台。
> 2. 定义了一个静态函数report_part_name，用于获取并输出NX部件的名称。它接收部件标识符part，并使用UF_PART_ask_part_name API获取部件名称，然后通过ECHO宏将名称输出到控制台。
> 3. RPN宏封装了report_part_name函数，提供了更简洁的调用方式，可以传入部件标识符，自动输出其名称。
> 4. 示例代码展示了如何使用NX Open C++ API获取部件信息，并通过宏封装简化调用，提高代码可读性和可维护性。
>
