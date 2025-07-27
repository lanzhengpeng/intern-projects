### 【0288】ask next parts list 询问下一个部件清单

#### 代码

```cpp
    /* 里海 */  
    static tag_t ask_next_parts_list(tag_t part, tag_t plist)  
    {  
        int  
            type = 0,  
            subtype = 0;  
        UF_OBJ_cycle_objs_in_part(part, UF_tabular_note_type, &plist);  
        if( NULL_TAG != plist )   
        {  
            UF_CALL( UF_OBJ_ask_type_and_subtype( plist, &type, &subtype ));  
            if( subtype == UF_parts_list_subtype ) return (plist);  
        }  
        return( NULL_TAG );  
    }

```

#### 代码解析

> 这段代码是在NX Open C++ API下开发的，其功能是遍历指定部件的表格注释，并获取部件的下一个零件列表。主要步骤包括：
>
> 1. 使用UF_OBJ_cycle_objs_in_part()函数遍历指定部件的表格注释对象，并将返回的对象列表存储在plist中。
> 2. 判断plist是否为空，若不为空，则继续。
> 3. 使用UF_OBJ_ask_type_and_subtype()函数获取plist的类型和子类型。
> 4. 判断plist的子类型是否为UF_parts_list_subtype，若是，则返回plist，表示找到了下一个零件列表。
> 5. 若不满足条件，则返回NULL_TAG，表示没有找到下一个零件列表。
>
> 这段代码的目的是在NX部件的表格注释中遍历查找零件列表，以实现表格注释的零件管理功能。
>
