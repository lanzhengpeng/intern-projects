### 【0973】create unique expression 创建唯一的表达式

#### 代码

```cpp
    /*HEAD CREATE_UNIQUE_EXPRESSION.CPP CCC UFUN */  
    #include <ug_exception.hxx>  
    #include <ug_session.hxx>  
    #include <ug_iterator.hxx>  
    #include <ug_expression.hxx>  
    #include <ug_info_window.hxx>  
    #include <uf.h>  
    static void do_it(void)  
    {  
        UgExpression *pExp;  
        pExp = UgExpression::createUnique("new_exp", "2+3");  
        pExp = UgExpression::createUnique("new_exp", 5.0);  
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

> 这段代码是NX Open C++的二次开发代码，主要实现了以下功能：
>
> 1. 定义了一个静态函数do_it，用于创建一个唯一的表达式对象pExp。通过调用UgExpression::createUnique接口，可以以字符串"2+3"或数值5.0作为参数创建表达式对象。
> 2. 在ufusr函数中，首先创建一个UgSession对象，用于与NX会话关联。
> 3. 调用do_it函数，创建唯一的表达式对象。
> 4. 使用try-catch结构处理可能抛出的UgException异常。如果捕获到异常，则打开一个UgInfoWindow窗口，并显示异常信息。
> 5. ufusr函数作为NX二次开发的入口函数，通过UG/Open提供的uf接口与NX集成。
> 6. 该代码展示了如何在NX二次开发中创建表达式对象，以及异常处理的基本用法。
>
