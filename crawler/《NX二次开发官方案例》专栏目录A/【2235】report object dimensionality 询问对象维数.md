### 【2235】report object dimensionality 询问对象维数

#### 代码

```cpp
    static void display_conehead(double origin[3], double vec[3])  
    {  
        UF_DISP_conehead(UF_DISP_ALL_ACTIVE_VIEWS, origin, vec, 0);  
    }  
    static void report_object_dimensionality(tag_t object)  
    {  
        int  
            dimensionality;  
        double  
            data[6];  
        UF_CALL(UF_MODL_ask_obj_dimensionality(object, &dimensionality, data));  
        switch (dimensionality)  
        {  
            case UF_MODL_POINT:  
                ECHO("Object is a point at %.3f, %.3f, %.3f\n",  
                    data[0], data[1], data[2]);  
                break;  
            case UF_MODL_LINEAR:  
                ECHO("Object is linear\n");  
                display_conehead(data, &data[3]);  
                break;  
            case UF_MODL_PLANAR:  
                ECHO("Object is planar\n");  
                display_conehead(data, &data[3]);  
                break;  
            case UF_MODL_THREE_DIMENSIONAL:  
                ECHO("Object is 3D\n");  
                break;  
            default:  
                ECHO("Object has unknown dimensionality (%d)\n", dimensionality);  
                break;  
        }  
    }

```

#### 代码解析

> 这段NX二次开发代码主要包含两个函数：
>
> 1. display_conehead函数：用于在当前激活的视图中显示圆锥头，接受圆锥头原点和方向向量作为参数。
> 2. report_object_dimensionality函数：用于报告NX对象的空间维度，接受对象tag作为参数。
>
> 函数工作流程如下：
>
> 1. 调用UF_MODL_ask_obj_dimensionality函数，获取对象的维度信息（点、线、面、体）和位置/方向数据。
> 2. 根据返回的维度信息，通过switch-case结构进行判断，并输出相应的信息。
> 3. 对于线或面，还会调用display_conehead函数在视图中显示圆锥头，以直观地指示对象的方向。
> 4. 对于未知维度，则输出错误信息。
> 5. 输出维度信息时，使用了格式化字符串以确保输出格式的一致性。
>
> 通过这段代码，用户可以快速了解NX对象的空间维度信息，并通过可视化的方式直观地查看线或面的方向，从而更好地理解模型。
>
