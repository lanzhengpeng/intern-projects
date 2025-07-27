### 【2911】specify plane 指定平面

#### 代码

```cpp
    static logical specify_plane(char *prompt, double orientation[3], double point[3])  
    {  
        int  
            mode = UF_UI_NO_DEFAULT,  
            resp;  
        tag_t  
            plane;  
        UF_CALL(UF_UI_specify_plane(prompt, &mode, UF_UI_DISP_TEMP_VECTOR, &resp,  
            orientation, point, &plane));  
        if (resp == UF_UI_OK) return TRUE;  
        else return FALSE;  
    }

```

#### 代码解析

> 这段代码是一个用于在NX中指定平面的静态逻辑函数，其主要功能如下：
>
> 1. 定义了函数specify_plane，它接受一个提示字符串、一个用于存放平面法向量的数组和一个用于存放平面点的数组。
> 2. 函数内部首先定义了一些变量，包括模式mode和响应resp，以及平面标签plane。
> 3. 调用了UF_UI_specify_plane函数，该函数用于提示用户在图形窗口中指定一个平面，并返回用户响应和指定的平面参数。函数参数包括提示字符串、模式、显示方式、响应、法向量和点数组以及平面标签。
> 4. 如果用户响应是确认，则函数返回真，否则返回假。
> 5. 该函数的作用是简化在NX中获取用户指定的平面参数的过程，将复杂的UF函数调用封装在一个简单的逻辑函数中，便于在其他代码中调用。
> 6. 使用静态关键字，意味着该函数可以被直接调用，不需要创建实例。
> 7. 整体上，该函数代码清晰简洁，通过UF函数调用获取用户输入，并通过返回值表示操作是否成功，是一个典型的NX二次开发辅助函数。
>
