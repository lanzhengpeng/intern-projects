### 【3054】wait for file to exist 等待文件存在

#### 代码

```cpp
    static void wait_for_file_to_exist(char *fspec)  
    {  
        int  
            not_there;  
        do  
        {  
            if (UF_CALL(UF_CFI_ask_file_exist(fspec, &not_there))) return;  
        } while (not_there);  
    }

```

#### 代码解析

> 这段代码是在NX Open C++ API中编写的一个函数，用于等待指定的文件存在。
>
> 主要思路是通过调用UF_CFI_ask_file_exist函数来询问文件是否存在，然后通过do…while循环不断询问，直到文件存在为止。
>
> 具体来说：
>
> 1. 定义了一个名为wait_for_file_to_exist的静态函数，参数是文件路径fspec。
> 2. 在函数体内，定义了一个整型变量not_there，用于接收文件是否存在的信息。
> 3. 使用do…while循环不断调用UF_CFI_ask_file_exist函数询问文件是否存在，并传入文件路径和not_there变量。
> 4. 如果文件不存在，not_there将被设置为非0值，循环继续执行。
> 5. 如果文件存在，not_there将被设置为0，循环结束。
> 6. 如果UF_CFI_ask_file_exist函数调用失败，函数直接返回。
> 7. 函数最终会在文件存在时返回。
>
> 总的来说，这个函数通过不断询问的方式，实现了等待文件存在的目的，适用于需要等待特定文件生成的场景。
>
