### 【1353】get work occurrence 获取工作出现

#### 代码

```cpp
    static tag_t get_work_occurrence()  
    {  
        tag_t  
            work_occ = UF_ASSEM_ask_work_occurrence();  
        if (work_occ == NULL_TAG)  
            work_occ = UF_ASSEM_ask_root_part_occ(UF_ASSEM_ask_work_part());  
        return work_occ;  
    }

```

#### 代码解析

> 这段代码是用于获取NX模型中当前工作部件的工作装配体标签。
>
> 主要步骤如下：
>
> 1. 使用UF_ASSEM_ask_work_occurrence()函数获取当前工作部件的工作装配体标签。如果获取到的标签为NULL_TAG，则表示当前工作部件没有工作装配体。
> 2. 如果获取到的标签为NULL_TAG，则调用UF_ASSEM_ask_root_part_occ()函数，传入当前工作部件的标签作为参数，获取当前工作部件的根装配体标签作为工作装配体标签。
> 3. 最后，返回工作装配体标签。
>
> 总的来说，这段代码的作用是确保获取到当前工作部件的工作装配体标签，如果当前工作部件没有工作装配体，则使用其根装配体作为工作装配体。
>
