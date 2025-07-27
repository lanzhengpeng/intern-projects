### 【2423】report view dependent objects 报告视图依赖对象

#### 代码

```cpp
    static void report_view_dependent_objects(char *view)  
    {  
        int  
            irc,  
            subtype,  
            type;  
        tag_t  
            object = NULL_TAG;  
        printf("view dependent objects in %s\n", view);  
        while (((irc = uc6457(view, 2, &object)) == 0) && (object != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(object, &type, &subtype));  
            printf("  tag = %d, type = %d, subtype = %d\n", object, type, subtype);  
        }  
        if (irc) ECHO(irc);  
    }

```

#### 代码解析

> 这段代码是NX Open C++ API编写的，用于报告指定视图中的所有依赖于视图的对象。
>
> 代码的主要功能包括：
>
> 1. 定义了一个静态函数report_view_dependent_objects，接收一个视图名称view作为参数。
> 2. 初始化一些变量，用于存储对象信息。
> 3. 使用printf函数输出提示信息。
> 4. 使用一个循环，调用uc6457函数获取视图中的依赖于视图的对象。如果获取成功且对象不为空，则进入循环体。
> 5. 在循环体中，使用UF_OBJ_ask_type_and_subtype函数获取对象的类型和子类型。
> 6. 使用printf函数输出对象的标记、类型和子类型。
> 7. 循环直到获取对象失败或对象为空。
> 8. 如果uc6457函数调用失败，使用ECHO函数输出错误码。
> 9. 函数结束。
>
> 这段代码的作用是遍历指定视图中的所有依赖于视图的对象，并输出它们的详细信息。依赖于视图的对象包括草图、构造几何等，这些对象仅存在于创建它们的视图之中。
>
> 代码示例清晰地展示了NX Open C++ API的基本用法，包括调用UF函数、处理返回值、循环遍历等。通过这段代码，可以了解如何在NX中遍历视图中的对象。
>
