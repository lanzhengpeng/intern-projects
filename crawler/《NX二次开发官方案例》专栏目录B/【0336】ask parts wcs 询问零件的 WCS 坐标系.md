### 【0336】ask parts wcs 询问零件的 WCS 坐标系

#### 代码

```cpp
    static tag_t ask_parts_wcs(tag_t part)  
    {  
        int  
            this_type,  
            this_subtype;  
        tag_t  
            csys = NULL_TAG;  
        while ((csys = UF_OBJ_cycle_all(part, csys)) && (csys != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(csys, &this_type, &this_subtype));  
            if ((this_type == UF_coordinate_system_type) &&   
                (this_subtype == UF_csys_wcs_subtype)) return csys;  
        }  
        return NULL_TAG;  //can't happen  
    }

```

#### 代码解析

> 这段NX二次开发代码的功能是遍历零件中的坐标系，并找到世界坐标系。
>
> 具体步骤如下：
>
> 1. 定义函数 ask_parts_wcs，接收零件tag作为参数。
> 2. 初始化坐标系tag为NULL_TAG。
> 3. 使用循环遍历零件中的所有坐标系，直到所有坐标系都遍历完毕。在循环中，使用UF_OBJ_cycle_all函数获取下一个坐标系。
> 4. 在每次循环中，使用UF_OBJ_ask_type_and_subtype函数获取坐标系的类型和子类型。
> 5. 判断坐标系的类型是否为UF_coordinate_system_type，子类型是否为UF_csys_wcs_subtype。如果是世界坐标系，则返回该坐标系的tag。
> 6. 如果遍历完所有坐标系后没有找到世界坐标系，则返回NULL_TAG。
>
> 通过这段代码，可以实现快速查找零件中的世界坐标系。
>
