### 【2951】test api execution 成功执行了测试API

#### 代码

```cpp
    /*HEAD LIST_ALL_FEATURES.CPP CCC UFUN */  
    #include <ug_exception.hxx>  
    #include <ug_session.hxx>  
    #include <ug_info_window.hxx>  
    #include <uf.h>  
    void ufusr(char *param, int *retcode, int paramLen)  
    {  
        try  
        {  
            UgSession my_session(true);  
            UgInfoWindow::open();  
            UgInfoWindow::write("Success!\n");  
        }  
        catch (UgException oops)  
        {  
            UF_print_syslog("*** Failure:  ", FALSE);  
            UF_print_syslog((char *) oops.askErrorText().c_str(), FALSE);  
            UF_print_syslog("\n", FALSE);  
        }  
    }

```

#### 代码解析

> 这段代码是NX二次开发的一个示例，其主要功能是打开UG信息窗口并显示“Success!”。代码使用UF函数接口实现，通过UF_print_syslog打印错误信息到系统日志。同时，它还使用了异常处理来捕获UgException异常，并在异常发生时打印错误信息。此外，代码还利用了UG会话，通过UgSession类来打开一个会话。这段代码的主要目的是展示如何使用UF函数、异常处理和UG会话在NX二次开发中。
>
