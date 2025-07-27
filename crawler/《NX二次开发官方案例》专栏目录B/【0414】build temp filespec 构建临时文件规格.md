### 【0414】build temp filespec 构建临时文件规格

#### 代码

```cpp
    static void build_temp_filespec(char *short_name, int ftype, char *fspec)  
    {  
        char  
            *tmp_dir;  
        UF_CALL(UF_translate_variable("UGII_TMP_DIR", &tmp_dir));  
        UF_CALL(uc4575(tmp_dir, ftype, short_name, fspec));  
    }

```

#### 代码解析

> 这段代码是NX Open C++ API的二次开发代码，其主要功能是构建一个临时文件的文件名。代码的具体介绍如下：
>
> 1. 函数名：build_temp_filespec。该函数用于构建一个临时文件的文件名。
> 2. 参数：short_name：输入参数，表示临时文件的基本名称。ftype：输入参数，表示临时文件的类型，例如UGII_TMP_DIR类型。fspec：输出参数，用于返回构建好的临时文件名。
> 3. short_name：输入参数，表示临时文件的基本名称。
> 4. ftype：输入参数，表示临时文件的类型，例如UGII_TMP_DIR类型。
> 5. fspec：输出参数，用于返回构建好的临时文件名。
> 6. 函数体：首先调用UF_translate_variable函数，获取NX系统临时目录的路径，并将其存储在tmp_dir指针所指向的字符串中。接着调用uc4575函数，将临时目录路径、文件类型、基本名称拼接成完整的临时文件名，并将其存储在fspec参数指向的字符串中。
> 7. 首先调用UF_translate_variable函数，获取NX系统临时目录的路径，并将其存储在tmp_dir指针所指向的字符串中。
> 8. 接着调用uc4575函数，将临时目录路径、文件类型、基本名称拼接成完整的临时文件名，并将其存储在fspec参数指向的字符串中。
> 9. 函数作用：通过调用NX系统函数，根据输入的基本名称和文件类型，在NX系统的临时目录下构建一个完整的临时文件名，以供后续使用。
>
> 这段代码在NX二次开发中用于生成临时文件，例如在读取或保存NX文件时，可能需要用到临时文件来存储中间数据。
>
