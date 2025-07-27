### 【0539】copy object 复制对象

#### 代码

```cpp
    static tag_t copy_object(tag_t object)  
    {  
        int  
            subtype,  
            this_subtype,  
            type;  
        tag_t  
            last = NULL_TAG,  
            next = NULL_TAG,  
            part = UF_ASSEM_ask_work_part();  
        char  
            part_name[MAX_FSPEC_SIZE+1];  
        UF_PART_export_options_t  
            options = { FALSE, UF_PART_remove_params, UF_PART_copy_exp_shallowly };  
    /*  find the last object in the work part of the same type 译:找到工作区中最后一个相同类型的对象 */  
        UF_CALL(UF_OBJ_ask_type_and_subtype(object, &type, &subtype));  
        while ((!UF_CALL(UF_OBJ_cycle_objs_in_part(part, type, &next))) && (next != NULL_TAG))  
            last = next;  
        UF_CALL(UF_PART_ask_part_name(part, part_name));  
        UF_CALL(UF_PART_export_with_options(part_name, 1, &object, &options));  
    /*  find the new copied object in the work part 译:在工作的部分找到新复制的对象 */  
        while (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, type, &last)) && (last != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(last, &type, &this_subtype));  
            if (this_subtype == subtype) break;  
        }  
        return last;  
    }

```

#### 代码解析

> 这段代码是NX Open API的二次开发代码，其主要功能是实现对象的复制。
>
> 主要流程包括：
>
> 1. 获取当前工作部件。
> 2. 获取要复制的对象类型和子类型。
> 3. 遍历工作部件，找到最后一个相同类型的对象。
> 4. 使用UF_PART_export_with_options函数，以浅拷贝的方式导出要复制的对象。
> 5. 再次遍历工作部件，找到新复制的对象，并返回其tag。
>
> 代码中使用了NX Open API中的UF_OBJ、UF_PART等模块，通过UF_OBJ_cycle_objs_in_part、UF_OBJ_ask_type_and_subtype等函数实现了对象的遍历和类型查询，以及UF_PART_export_with_options实现了对象的导出和复制。
>
> 总体来说，这段代码实现了在NX环境中复制对象的功能，通过API调用实现了对象查询、遍历、导出等操作，实现了对象的复制。
>
