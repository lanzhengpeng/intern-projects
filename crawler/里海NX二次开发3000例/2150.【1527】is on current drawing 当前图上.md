### 【1527】is on current drawing 当前图上

#### 代码

```cpp
    static logical is_on_current_drawing(tag_t object)  
    {  
        int  
            status;  
        char  
            dname[MAX_ENTITY_NAME_SIZE+1],  
            view[MAX_ENTITY_NAME_SIZE+1];  
        uc6492(dname);  
        strcat(dname, "@0");  
        uc6409(object, &status, view);  
        if (!status || strcmp(view, dname)) return FALSE;  
        return TRUE;  
    }

```

#### 代码解析

> 这段代码是NX Open C++ API编写的二次开发代码，用于判断一个NX对象是否在当前的图纸上。主要功能包括：
>
> 1. 定义了一个静态逻辑函数is_on_current_drawing，接受一个NX对象的tag作为参数。
> 2. 获取当前图纸的名称，并附加@0以构成完整的图纸路径。
> 3. 使用NX API函数uc6409查询传入对象的视图状态和所在图纸路径。
> 4. 判断对象的状态是否为1(表示在图纸中)，以及所在图纸路径是否与当前图纸路径相同。
> 5. 如果对象状态为1且所在图纸路径与当前图纸路径相同，则返回true，表示对象在当前图纸上；否则返回false。
> 6. 通过调用该函数，可以判断一个NX对象是否在当前图纸上，从而进行相应的处理。
>
> 这段代码展示了如何利用NX的底层API来实现对象与图纸关系的判断，为NX二次开发提供了实用的功能。
>
