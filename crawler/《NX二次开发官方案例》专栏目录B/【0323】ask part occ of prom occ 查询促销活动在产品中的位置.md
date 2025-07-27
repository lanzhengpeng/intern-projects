### 【0323】ask part occ of prom occ 查询促销活动在产品中的位置

#### 代码

```cpp
    static tag_t ask_part_occ_of_prom_occ(tag_t prom_object)  
    {  
        tag_t  
            owning_part,  
            part_occ = prom_object,  
            proto;  
        proto = UF_ASSEM_ask_prototype_of_occ(prom_object);  
        UF_CALL(UF_OBJ_ask_owning_part(proto, &owning_part));  
        while (!UF_CALL(UF_ASSEM_ask_parent_component(part_occ, &part_occ))  
            && (UF_ASSEM_ask_prototype_of_occ(part_occ) != owning_part));  
        return part_occ;  
    }

```

#### 代码解析

> 这段代码是一个NX Open C++ API函数，其功能是获取给定程序集实例的装配部件的实例。具体步骤如下：
>
> 1. 首先，获取程序集实例的prototype原型。
> 2. 接着，通过原型获取其拥有部件。
> 3. 然后，遍历当前程序集实例的父组件，直到找到拥有部件的实例。
> 4. 最后，返回找到的拥有部件的实例。
>
> 这个函数主要用于在NX装配环境中，根据给定的程序集实例，找到其对应的装配部件实例。通过原型和父组件的遍历，可以准确定位到目标实例。
>
