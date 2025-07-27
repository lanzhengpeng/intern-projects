### 【1290】find parent csys of wcs 找到WCS的父坐标系

#### 代码

```cpp
    static tag_t find_parent_csys_of_wcs( tag_t wcs )  
    {  
    /*  
        The WCS is a type 45, subtype 1.  
        A normal CSYS entity is a type 45, subtype 0.  
        Pass in a WCS, and if it is based on an existing normal CSYS, entity,  
        the tag for the csys entity will be returned.  
    */    int  
            type = 0,  
            subtype = 0;  
        tag_t  
            wcs_mx_id = NULL_TAG,  
            csys_mx_id = NULL_TAG,  
            csys_tag = NULL_TAG,  
            part = UF_PART_ask_display_part();  
        double  
            wcs_origin[3],  
            csys_origin[3],  
            wcs_mx[9],  
            csys_mx[9];  
        UF_CALL(UF_CSYS_ask_csys_info( wcs, &wcs_mx_id, wcs_origin ));   
        UF_CALL(UF_CSYS_ask_matrix_values( wcs_mx_id, wcs_mx ));  
        while(!UF_CALL(UF_OBJ_cycle_objs_in_part( part,   
                UF_coordinate_system_type, &csys_tag )) && NULL_TAG != csys_tag )  
            {  
                UF_CALL( UF_OBJ_ask_type_and_subtype( csys_tag, &type, &subtype ));  
                if( subtype == 1 ) break;  
                UF_CALL(UF_CSYS_ask_csys_info( csys_tag, &csys_mx_id, csys_origin ));   
                UF_CALL(UF_CSYS_ask_matrix_values( csys_mx_id, csys_mx ));  
                if(  
                  ( wcs_origin[0] == csys_origin[0] ) &&  
                  ( wcs_origin[1] == csys_origin[1] ) &&  
                  ( wcs_origin[2] == csys_origin[2] ) &&  
                  ( wcs_mx[0] == csys_mx[0] ) &&  
                  ( wcs_mx[1] == csys_mx[1] ) &&  
                  ( wcs_mx[2] == csys_mx[2] ) &&  
                  ( wcs_mx[3] == csys_mx[3] ) &&  
                  ( wcs_mx[4] == csys_mx[4] ) &&  
                  ( wcs_mx[5] == csys_mx[5] ) &&  
                  ( wcs_mx[6] == csys_mx[6] ) &&  
                  ( wcs_mx[7] == csys_mx[7] ) &&  
                  ( wcs_mx[8] == csys_mx[8] )  
                  ) return( csys_tag );  
            }  
            return( NULL_TAG );  
    }

```

#### 代码解析

> 这段代码的主要功能是在NX中查找与给定的工作坐标系(WCS)相匹配的普通坐标系(CSYS)。具体步骤如下：
>
> 1. 获取输入WCS的矩阵ID和原点信息。
> 2. 遍历当前零件中的所有CSYS实体，获取每个CSYS的类型和子类型，如果是普通CSYS(子类型为0)则继续。
> 3. 获取该CSYS的矩阵ID和原点信息。
> 4. 比较WCS和CSYS的原点坐标和矩阵值，如果完全相同，则返回该CSYS的标签。
> 5. 如果遍历完所有CSYS后没有找到匹配的，则返回空标签。
>
> 通过比较WCS和CSYS的参数来判断它们是否相同，如果找到匹配的CSYS，则返回其标签。这段代码可用于在NX中查找与WCS对应的普通CSYS。
>
