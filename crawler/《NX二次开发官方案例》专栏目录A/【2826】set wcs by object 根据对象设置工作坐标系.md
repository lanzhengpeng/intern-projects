### 【2826】set wcs by object 根据对象设置工作坐标系

#### 代码

```cpp
    static void set_wcs_by_object(tag_t object)  
    {  
        int  
            subtype,  
            type;  
        tag_t  
            csys,  
            mx;  
        double  
            origin[3] = { 0.0, 0.0, 0.0 };  
        UF_CALL(UF_OBJ_ask_type_and_subtype(object, &type, &subtype));  
        if (subtype == UF_solid_edge_subtype)  
            UF_CALL(UF_MODL_create_curve_from_edge(object, &object));  
        UF_CALL(UF_CSYS_ask_matrix_of_object(object, &mx));  
        if (mx != NULL_TAG)  
        {  
            UF_CALL(UF_CSYS_create_temp_csys(origin, mx, &csys));  
            UF_CALL(UF_CSYS_set_wcs(csys));  
        }  
        else  
        {  
            uc1601("Object has no matrix", TRUE);  
        }  
        if (subtype == UF_solid_edge_subtype)  
            UF_CALL(UF_OBJ_delete_object(object));  
    }

```

#### 代码解析

> 这段代码的主要功能是根据NX对象设置工作坐标系(WCS)。
>
> 关键点如下：
>
> 1. 首先，调用UF_OBJ_ask_type_and_subtype函数获取对象类型和子类型。如果对象是实体边缘，则调用UF_MODL_create_curve_from_edge将其转换为曲线，因为实体边缘没有坐标系，而曲线有。
> 2. 然后，调用UF_CSYS_ask_matrix_of_object获取对象的坐标系矩阵。如果存在坐标系矩阵，则使用UF_CSYS_create_temp_csys创建一个临时坐标系，并将对象的坐标系矩阵作为该临时坐标系的数据。
> 3. 接下来，调用UF_CSYS_set_wcs将临时坐标系设置为WCS。
> 4. 最后，如果对象是实体边缘，则在设置WCS后删除该对象。
> 5. 代码中使用了UF_CALL宏来简化函数调用，它会自动检查函数返回码。
> 6. 如果对象没有坐标系矩阵，则会弹出一个错误消息框。
>
> 这段代码的作用是根据NX对象自动设置WCS，为后续操作提供方便。
>
