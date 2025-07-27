### 【2942】swap all components reference set 交换所有组件的参考集

#### 代码

```cpp
    static void swap_all_components_reference_set(char *refset)  
    {  
        tag_t  
            cset,  
            part = UF_ASSEM_ask_work_part();  
        UF_CALL(UF_ASSEM_ask_all_comp_cset(part, &cset));  
        UF_CALL(UF_ASSEM_set_ref_set_by_cset(cset, refset));  
    }

```

#### 代码解析

> 这段代码是用于NX Open C++ API的二次开发代码，其功能是设置NX装配组件的参考集。具体来说，代码的主要步骤包括：
>
> 1. 获取当前工作部件的装配组件集合cset。
> 2. 获取当前工作的部件part。
> 3. 设置参考集为字符数组refset。
> 4. 通过UF_ASSEM_set_ref_set_by_cset函数，将cset中的所有组件设置为refset指定的参考集。
>
> 因此，这段代码的目的是将NX装配模型中当前工作部件的所有组件设置为指定的参考集，以便后续的引用操作，如重定位、复制等。
>
