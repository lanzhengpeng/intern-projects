### 【0505】build util filespec 构建实用文件规格

#### 代码

```cpp
    static void build_util_filespec(char *short_name, int ftype, char *fspec)  
    {  
        char  
            *util_dir;  
        UF_CALL(UF_translate_variable("UGII_UTIL", &util_dir));  
        UF_CALL(uc4575(util_dir, ftype, short_name, fspec));  
    }

```

#### 代码解析

> 这段代码是用于NX二次开发的，主要功能是构建一个文件路径，该路径指向NX的实用程序文件夹下的一个文件。
>
> 代码首先调用UF_translate_variable函数，获取NX的实用程序文件夹路径，并将路径保存在util_dir指针所指向的字符串中。
>
> 接着，调用uc4575函数，该函数使用util_dir路径，结合传入的文件类型ftype和文件简称short_name，拼接出完整的文件路径，并将路径保存在fspec指针所指向的字符串中。
>
> 其中，ftype表示文件类型，short_name表示文件简称，fspec表示完整的文件路径。
>
> 总之，这段代码通过NX的实用程序文件夹路径、文件类型和文件简称，构建出完整的文件路径，用于后续的文件操作。
>
