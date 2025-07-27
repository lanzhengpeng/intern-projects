### 【1933】report all named objects 报告所有命名对象

#### 代码

```cpp
    /*HEAD REPORT_ALL_NAMED_OBJECTS.CPP CCC UFUN */  
    #include <ug_exception.hxx>  
    #include <ug_session.hxx>  
    #include <ug_typed.hxx>  
    #include <ug_part.hxx>  
    #include <ug_info_window.hxx>  
    static void do_it(UgPart *part)  
    {  
        UgTypedObject  
            *typed_object;  
        UgInfoWindow  
            info;  
        info.open();  
        for ( typed_object = part->iterateFirst();  
              typed_object;  
              typed_object = part->iterateNext(typed_object) )  
        {  
            std::string ObjectName = typed_object->getName();  
            if ( ObjectName != "")  
            {  
                info.write(ObjectName);  
                info.write("\n");  
            }  
        }  
    }  
    void ufusr(char *param, int *retcode, int paramLen)  
    {  
        UgSession my_session(true);  
        try  
        {  
            UgPart *part = my_session.getWorkPart();  
            do_it(part);  
        }  
        catch (UgException oops)  
        {  
            oops.processError();  
        }  
    }

```

#### 代码解析

> 这段代码是NX Open C++的一个二次开发示例，其主要功能是遍历当前工作部件中的所有命名对象并输出它们的名称。
>
> 主要逻辑包括：
>
> 1. 包含必要的NX Open C++头文件。
> 2. 定义了一个静态函数do_it，用于遍历部件中的所有对象，并输出对象的名称（如果存在）。
> 3. 在ufusr函数中，获取当前会话的工作部件，然后调用do_it函数。
> 4. 使用try-catch块捕获并处理可能出现的异常。
> 5. ufusr函数是NX Open C++二次开发程序的入口点，其中param和retcode参数由NX调用时传入。
> 6. 整个程序以ANSI C++标准编写。
>
> 通过这段代码，用户可以了解如何在NX Open C++中进行基本的二次开发，包括会话管理、对象遍历和异常处理。
>
