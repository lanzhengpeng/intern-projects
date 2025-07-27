### 【2241】report object name 报告对象名称

#### 代码

```cpp
    #define RON(X) (report_object_name(#X, X))  
    static void report_object_name(char *what, tag_t object)  
    {  
        char  
            object_name[MAX_ENTITY_NAME_SIZE+1];  
        if (!UF_OBJ_ask_name(object, object_name))  
            ECHO("%s is named %s\n", what, object_name);  
        else  
            ECHO("%s is not named\n", what);  
    }

```

#### 代码解析

> 这段代码定义了一个宏 RON，用于报告 NX 对象的名称。其工作原理如下：
>
> 1. 宏 RON(X) 定义为调用 report_object_name 函数，并传递字符串字面量 #X（即变量 X 的名称）和变量 X 本身作为参数。
> 2. report_object_name 函数接受两个参数：一个字符指针 what（指向字符串，即传入的变量名）和一个 NX 对象 tag_t 类型的对象。
> 3. 该函数使用 UF_OBJ_ask_name 函数获取对象名称，并将结果存储在局部字符数组 object_name 中。若获取成功，则打印输出对象的名称；否则，打印输出对象未命名。
> 4. UF_OBJ_ask_name 是 NX 提供的 API 函数，用于查询对象的名称。
> 5. ECHO 是预定义的宏，用于打印输出字符串。
> 6. MAX_ENTITY_NAME_SIZE 是 NX 中定义的常量，表示对象名称的最大长度。
>
> 综上所述，该代码通过宏和函数的配合，提供了查询和报告 NX 对象名称的便捷方式。
>
