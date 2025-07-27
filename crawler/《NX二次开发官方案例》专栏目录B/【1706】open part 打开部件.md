### 【1706】open part 打开部件

#### 代码

```cpp
    /*HEAD OPEN_PART.CXX CCC UFUN */  
    #include <ug_exception.hxx>  
    #include <ug_session.hxx>  
    #include <ug_part.hxx>  
    #include <ug_info_window.hxx>  
    static void do_it()  
    {  
        UgPart *part = UgPart::open("/users/harbin/test.prt");  
    }  
    void ufusr(char *param, int *retcode, int paramLen)  
    {  
        try  
        {  
            UgSession my_session(true);  
            do_it();  
        }  
        catch (UgException oops)  
        {  
            UgInfoWindow  info;  
            info.open();  
            info.write(oops.askErrorText());  
            info.write("\n");  
            return;  
        }  
    }

```

#### 代码解析

> 这段代码是一段用于在NX中打开零件的二次开发代码，主要功能包括：
>
> 1. 引入了UG NX二次开发所需的头文件，包括异常处理、会话管理和零件管理等头文件。
> 2. 定义了一个静态函数do_it()，用于打开指定路径的零件文件。
> 3. 定义了ufusr()函数，这是NX二次开发中的主函数。在该函数中：创建了一个UgSession对象，用于与NX建立会话。在try块中调用do_it()函数以打开零件文件。使用catch块来捕获并处理UgException异常，如果打开零件时发生异常，则显示错误信息。
> 4. 创建了一个UgSession对象，用于与NX建立会话。
> 5. 在try块中调用do_it()函数以打开零件文件。
> 6. 使用catch块来捕获并处理UgException异常，如果打开零件时发生异常，则显示错误信息。
> 7. ufusr()函数通过NX提供的参数和返回码参数与NX进行交互。
>
> 总的来说，这段代码实现了在NX中打开指定路径零件文件的功能，并通过异常处理来确保程序稳定性。
>
