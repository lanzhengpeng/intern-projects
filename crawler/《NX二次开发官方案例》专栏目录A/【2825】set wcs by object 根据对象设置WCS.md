### 【2825】set wcs by object 根据对象设置WCS

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

> 这是段NX二次开发代码，主要功能是设置工作坐标系(WCS)为指定对象(object)的坐标系。具体步骤如下：
>
> 1. 获取对象类型和子类型，并判断是否为实体边，如果是，则转换为曲线对象。
> 2. 获取对象的坐标系矩阵。
> 3. 如果有坐标系矩阵，则根据矩阵和原点创建一个临时坐标系，并设置为WCS。
> 4. 如果没有坐标系矩阵，则提示错误。
> 5. 最后，如果对象是实体边，则删除转换得到的曲线对象。
>
> 这段代码主要用于在NX中根据指定对象的坐标系设置WCS，以便后续操作基于该对象的坐标系进行。
>
