### 【0684】create duplicate copy of object 创建对象的副本

#### 代码

```cpp
    static tag_t create_duplicate_copy_of_object(tag_t object)  
    {  
        int  
            ii,  
            n_ref_sets,  
            one = 1,  
            resp,  
            two = 2,  
            zero = 0;  
        tag_t  
            group,  
            new_object,  
            *ref_sets;  
        double  
            mx[12],  
            zero_delta[3] = { 0, 0, 0 };  
        FTN(uf5943)(zero_delta, mx);  
        FTN(uf5947)(mx, &object, &one, &two, &zero, &two, &new_object, NULL, &resp);  
        if (resp != 0)  
        {  
            ECHO(resp);  
            return NULL_TAG;  
        }  
    /*  Transformed copy will not be in group of original 译:The transformed copy will not be in the group of the original. */  
        UF_CALL(UF_GROUP_ask_group_of_tag(object, &group));  
        if (group != NULL_TAG)  
            UF_CALL(UF_GROUP_add_member_to_group(new_object, group));  
    /*  Transformed copy will not be in reference sets of original 译:翻译后的副本不会在原始的参考集中。 */  
        UF_CALL(UF_ASSEM_ask_ref_sets(object, &n_ref_sets, &ref_sets));  
        if (n_ref_sets > 0)  
        {  
            for (ii = 0; ii < n_ref_sets; ii++)  
                UF_CALL(UF_ASSEM_add_ref_set_members(ref_sets[ii], 1, &new_object));  
            UF_free(ref_sets);  
        }  
        return new_object;  
    }

```

#### 代码解析

> 这段代码的主要功能是创建一个NX对象(如部件、特征等)的副本。代码流程如下：
>
> 1. 调用FTN(uf5943)函数，计算一个零变换矩阵mx。
> 2. 调用FTN(uf5947)函数，使用零变换矩阵mx将原始对象object进行变换，创建一个新的副本对象new_object。
> 3. 如果原始对象属于某个组group，则将副本对象也加入到这个组中。
> 4. 查询原始对象所在的参考集ref_sets，并将副本对象也加入到这些参考集中。
> 5. 返回创建的副本对象new_object。
>
> 总的来说，这段代码通过变换创建了一个原始对象的副本，并将副本加入到原始对象的组和参考集中，以保证副本与原始对象保持一致。
>
