### 【0367】ask ug integer version 询问UG整数版本

#### 代码

```cpp
    static int ask_ug_integer_version(void)  
    {  
        int  
            rel_no;  
        char  
            *release;  
        UF_CALL(UF_get_release(&release));  
        rel_no = atoi(release+1);  
        UF_free(release);  
        return rel_no;  
    }

```

#### 代码解析

> 这段代码用于查询NX软件的版本号。
>
> 主要步骤包括：
>
> 1. 调用UF_get_release()函数获取NX软件的版本字符串。
> 2. 从版本字符串中提取版本号，版本字符串格式为"V10.0.2.4"。
> 3. 使用atoi()函数将提取的版本号转换为整数。
> 4. 返回转换后的整数版本号。
> 5. 释放版本字符串内存。
>
> 这段代码的主要功能是获取NX软件的版本号，并返回一个整数表示的版本号。通过调用UF_get_release()函数获取版本字符串，然后提取并转换为整数。这样方便在其他代码中根据版本号进行条件判断。
>
