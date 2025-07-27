### 【3006】turn off loading of components 关闭组件加载

#### 代码

```cpp
    static void turn_off_loading_of_components(void)  
    {  
        UF_ASSEM_options_t  
            assy_options;  
        UF_CALL(UF_ASSEM_ask_assem_options(&assy_options));  
        if (assy_options.load_components != UF_ASSEM_dont_load_components)  
        {  
            assy_options.load_components = UF_ASSEM_dont_load_components;  
            UF_CALL(UF_ASSEM_set_assem_options(&assy_options));  
        }  
    }

```

#### 代码解析

> 这段代码是NX Open API中用于控制装配加载组件的示例代码。其主要功能是关闭NX装配的组件加载功能。
>
> 具体步骤如下：
>
> 1. 定义一个UF_ASSEM_options_t类型的变量assy_options，用于存储装配选项。
> 2. 调用UF_ASSEM_ask_assem_options()函数，获取当前的装配选项并存储在assy_options变量中。
> 3. 检查assy_options变量中的load_components字段，如果其值不是UF_ASSEM_dont_load_components，则说明当前装配组件是加载的。
> 4. 如果load_components不是关闭状态，则将其设置为UF_ASSEM_dont_load_components，表示关闭组件加载功能。
> 5. 调用UF_ASSEM_set_assem_options()函数，将修改后的assy_options设置回NX装配，从而实现关闭组件加载的功能。
>
> 总之，这段代码通过获取和设置NX装配的选项，实现了关闭组件加载的功能。这在二次开发中，可以在打开大型装配时节省内存和加快速度。
>
