### 【1848】register function to run after file new 注册一个在文件新建后运行的函数

#### 代码

```cpp
    /*HEAD REGISTER_FUNCTION_TO_RUN_AFTER_PART_OPEN CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <stdlib.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_part.h>  
    #include <uf_exit.h>  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133],  
                 msg[133];  
            sprintf(msg, "*** ERROR code %d at line %d in %s:\n+++ ",  
                irc, line, file);  
            UF_get_fail_message(irc, err);  
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档，UF_print_syslog 是 V18 新增的函数。

在文档中指出：

```plaintext
NOTE: UF_print_syslog is new in V18

```

```cpp
# 代码解析
这段代码是NX的二次开发代码，主要实现了以下功能：

1. 定义了一个错误报告函数report_error，用于在发生UF函数调用错误时，打印错误信息到系统日志和NX界面。

2. 实现了一个回调函数my_create_callback，当NX创建新零件时，该函数会被调用，并打印新零件的名称到NX界面。

3. 在ufsta函数中，首先调用UF_initialize初始化UF库，然后调用UF_add_callback_function注册my_create_callback函数，使其成为NX创建新零件时的回调函数。最后调用UF_terminate终止UF库。

4. ufsta函数的目的是在NX打开新零件后，自动注册回调函数，以便在后续创建新零件时能够打印零件名称。

5. 整个代码的主要功能是监控NX创建新零件，并在创建后打印零件名称，方便用户查看。通过注册回调函数来实现对NX事件的响应。

6. 注释中提到UF_print_syslog是NX 18新增的函数，说明代码可能适用于NX 18及以上版本。

7. 通过二次开发，实现了对NX内部事件的监控和响应，扩展了NX的功能。
![是里海啊](https://img-blog.csdnimg.cn/4c206213ad7b4596bd2a216c390652ce.gif#pic_center)


```

#### 代码解析
> 未找到解析内容

