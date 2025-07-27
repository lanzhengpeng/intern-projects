### 【0240】ask immediate occs of part 查询部件的直接出现

#### 代码

```cpp
    static int ask_immediate_occs_of_part(tag_t parent, tag_t child, tag_t **i_occs)  
    {  
        int  
            ii,  
            n;  
        tag_t  
            *occs,  
            root_occ;  
        uf_list_p_t  
            i_occ_list;  
        UF_CALL(UF_MODL_create_list(&i_occ_list));  
        root_occ = UF_ASSEM_ask_root_part_occ(parent);  
        n = UF_ASSEM_ask_occs_of_part(parent, child, &occs);  
        for (ii = 0; ii < n; ii++)  
        {  
            if (UF_ASSEM_ask_part_occurrence(occs[ii]) == root_occ)  
                UF_CALL(UF_MODL_put_list_item(i_occ_list, occs[ii]));  
        }  
        if (n > 0) UF_free(occs);  
        return make_an_array(&i_occ_list, i_occs);  
    }

```

#### 代码解析

> 这段代码实现了在NX Open中询问一个部件在装配中的直接实例的功能。
>
> 主要步骤如下：
>
> 1. 创建一个列表i_occ_list，用于存放直接实例。
> 2. 获取父部件的根实例root_occ。
> 3. 获取父部件中所有子部件的实例，并存放在数组occs中。
> 4. 遍历数组occs，如果实例的根实例等于父部件的根实例，则将该实例添加到列表i_occ_list中。
> 5. 如果数组occs不为空，则释放数组occs。
> 6. 将列表i_occ_list转换为数组*i_occs，并返回数组的大小。
>
> 通过这段代码，可以方便地获取一个部件在装配中的直接实例，并进行进一步处理。
>
