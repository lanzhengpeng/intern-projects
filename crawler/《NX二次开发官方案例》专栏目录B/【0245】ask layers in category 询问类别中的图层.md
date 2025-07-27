### 【0245】ask layers in category 询问类别中的图层

#### 代码

```cpp
    static tag_t ask_category_tag(char *name)  
    {  
    /*  Prior to V18 - do this instead  
        tag_t  
            object = NULL_TAG,  
            part = UF_PART_ask_display_part();  
        UF_CALL(UF_OBJ_cycle_by_name_and_type(part, name, UF_layer_category_type,  
            FALSE, &object));  
    */  
        tag_t  
            object;  
        UF_CALL(UF_LAYER_ask_category_tag(name, &object));  
        return object;  
    }  
    static int allocate_memory(unsigned int nbytes, void **where)  
    {  
        int  
            resp;  
        *where = UF_allocate_memory(nbytes, &resp);  
        return resp;  
    }  
    static int ask_layers_in_category(char *category, int **layers)  
    {  
        int  
            n_layers = 0,  
            ii,  
            jj;  
        tag_t  
            cat_tag = ask_category_tag(category);  
        UF_LAYER_category_info_t  
            category_info;  
        if (cat_tag == NULL_TAG) return 0;  
        UF_CALL(UF_LAYER_ask_category_info(cat_tag, &category_info));  
        for (ii = 0; ii < UF_LAYER_MAX_LAYER; ii++)  
            if (category_info.layer_mask[ii] == TRUE) n_layers++;  
        UF_CALL(allocate_memory(n_layers * sizeof(int), (void *)layers));  
        for (ii = 0, jj = 0; ii < UF_LAYER_MAX_LAYER; ii++)  
            if (category_info.layer_mask[ii] == TRUE)  
                (*layers)[jj++] = ii + 1;  
        return n_layers;  
    }

```

#### 代码解析

> 这段代码是NX Open C++ API编写的二次开发代码，其主要功能是查询NX中的图层类别信息并获取该类别下的所有图层。
>
> 具体步骤包括：
>
> 1. 查询图层类别标签：使用ask_category_tag函数，通过图层类别名称查询其标签。
> 2. 分配内存：使用allocate_memory函数，根据图层数量分配内存，用于存储图层编号。
> 3. 查询图层信息：使用ask_layers_in_category函数，根据类别标签查询该类别下的所有图层编号，并将这些编号存储在数组中返回。
> 4. 兼容性说明：在代码注释中提到，在NX 18版本之前，需要使用UF_OBJ_cycle_by_name_and_type函数来查询图层类别，而在NX 18及之后版本中，直接使用UF_LAYER_ask_category_tag函数即可。
> 5. 返回结果：最终，ask_layers_in_category函数返回了指定类别下的所有图层编号数组及图层数量。
>
> 总的来说，这段代码提供了查询NX中指定图层类别下所有图层编号的功能。
>
