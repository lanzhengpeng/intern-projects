### 【2730】set assembly layers like component 设置装配层与组件相同

#### 代码

```cpp
    static void set_assembly_layers_like_component( tag_t assy_part, tag_t comp_part )  
    {  
        int  
            inx = 1,  
            layer_status[257];  
        for( inx = 1; inx < 257; inx++ )  
        {  
            UF_CALL( UF_PART_set_display_part( comp_part ));  
            UF_CALL( UF_LAYER_ask_status ( inx, &layer_status[inx] ));  
        }  
            UF_CALL( UF_PART_set_display_part( assy_part ));  
        for( inx = 1; inx < 257; inx++ )  
        {  
            UF_CALL( UF_LAYER_set_status( inx , layer_status[inx] ));  
        }  
    }

```

#### 代码解析

> 这段NX二次开发代码的目的是将一个组件的图层状态设置到另一个装配体零件上。
>
> 主要步骤包括：
>
> 1. 定义一个数组layer_status来存储组件的图层状态。
> 2. 遍历组件的图层，将每个图层的显示状态存入layer_status数组中。
> 3. 切换到装配体零件。
> 4. 遍历装配体零件的图层，根据layer_status数组设置每个图层的显示状态。
> 5. 通过UF_PART_set_display_part函数来切换当前显示的零件。
> 6. 使用UF_LAYER_ask_status函数来获取图层的显示状态。
> 7. 使用UF_LAYER_set_status函数来设置图层的显示状态。
>
> 总的来说，这段代码通过存储组件图层的显示状态，然后将这些状态复制到装配体零件的图层，实现了组件和装配体图层显示的统一。
>
