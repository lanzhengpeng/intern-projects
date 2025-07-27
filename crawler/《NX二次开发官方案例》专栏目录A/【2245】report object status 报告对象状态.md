### 【2245】report object status 报告对象状态

#### 代码

```cpp
    static void report_object_status(tag_t object_id)  
    {  
        int  
            status = UF_OBJ_ask_status(object_id);  
        switch(status)  
        {  
            case UF_OBJ_DELETED:  
                ECHO("object tag = %d is UF_OBJ_DELETED = %d\n", object_id, status);  
                break;  
            case UF_OBJ_TEMPORARY:  
                ECHO("object tag = %d is UF_OBJ_TEMPORARY = %d\n", object_id, status);  
                break;  
            case UF_OBJ_CONDEMNED:  
                ECHO("object tag = %d is UF_OBJ_CONDEMNED = %d\n", object_id, status);  
                break;  
            case UF_OBJ_ALIVE:  
                ECHO("object tag = %d is UF_OBJ_ALIVE = %d\n", object_id, status);  
                break;  
            default:  
                break;  
        }  
    }

```

#### 代码解析

> 这段代码是NX Open C++ API编写的，主要功能是查询和报告NX中指定对象的状态。
>
> 代码首先调用UF_OBJ_ask_status函数，传入对象ID(object_id)作为参数，获取对象的状态。然后，根据状态值，使用switch语句输出不同状态对应的提示信息。
>
> 具体来说，代码可以查询对象是否被删除(UF_OBJ_DELETED)，是否为临时对象(UF_OBJ_TEMPORARY)，是否被废弃(UF_OBJ_CONDEMNED)，或者是否正常(UF_OBJ_ALIVE)。其中，UF_OBJ_ALIVE表示对象正常存在。
>
> 此外，代码使用了ECHO宏来打印提示信息，它会在NX日志窗口中输出信息。同时，代码结构清晰，逻辑简单，通过简单的调用和判断，实现了查询和报告对象状态的功能。
>
> 总的来说，这段代码是一个简单的示例，展示了如何使用NX Open C++ API来获取对象的状态信息。
>
