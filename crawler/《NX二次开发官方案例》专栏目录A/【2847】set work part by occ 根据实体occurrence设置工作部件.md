### 【2847】set work part by occ 根据实体occurrence设置工作部件

#### 代码

```cpp
    static void set_work_part_by_occ(tag_t ent_occ)  
    {  
        UF_CALL(UF_ASSEM_set_work_part(  
            UF_ASSEM_ask_prototype_of_occ(  
            UF_ASSEM_ask_part_occurrence(ent_occ))));  
    }

```

#### 代码解析

> 这段代码是用于NX二次开发中的函数，其作用是设置当前的工作部件。具体来说，函数接受一个实体出现的参数ent_occ，通过调用UF_ASSEM_ask_prototype_of_occ函数获取该实体出现的原型，然后调用UF_ASSEM_set_work_part函数将其设置为当前的工作部件。
>
> 通过设置工作部件，可以使得后续的NX操作都针对该部件进行。这为NX的二次开发提供了灵活性，能够根据需要切换和操作不同的部件。
>
> 这段代码是典型的NX二次开发中的函数，通过调用NX提供的API函数来实现所需功能。作为作者，这段代码的设计考虑了NX的API调用，实现了通过实体出现设置工作部件的功能。
>
