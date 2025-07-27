### 【0360】ask temp directory 问临时目录

#### 代码

```cpp
    static void ask_temp_directory(char *fspec)  
    {  
        char  
            *t_dir;  
        UF_CALL(UF_translate_variable("UGII_TMP_DIR", &t_dir));  
        strcpy(fspec, t_dir);  
    }

```

#### 代码解析

> 这段代码是NX的二次开发代码，主要用于实现以下功能：
>
> 1. 定义了一个名为ask_temp_directory的静态函数，用于获取NX的临时目录路径。
> 2. 函数内部首先调用UF_translate_variable函数，将NX中的系统变量UGII_TMP_DIR翻译成实际的路径字符串，并存储在t_dir指针中。
> 3. 然后使用strcpy函数，将t_dir中的路径字符串复制到fspec参数指向的字符串中，从而实现了获取NX临时目录路径的功能。
> 4. 这个函数可以方便地在NX二次开发中获取临时目录路径，以便于进行临时文件的读写等操作。
>
> 总的来说，这段代码实现了在NX二次开发中获取临时目录路径的功能，是NX二次开发常用技巧之一。
>
