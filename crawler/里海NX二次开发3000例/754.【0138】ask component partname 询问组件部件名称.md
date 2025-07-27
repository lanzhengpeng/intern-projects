### 【0138】ask component partname 询问组件部件名称

#### 代码

```cpp
    static void ask_component_partname(tag_t component, char *partname)  
    {  
        tag_t  
            instance;  
        double  
            csys_matrix[9],  
            origin[3],  
            transform[4][4];  
        char  
            instance_name[UF_CFI_MAX_FILE_NAME_SIZE],  
            refset_name[MAX_ENTITY_NAME_SIZE+1];  
    /*  This returns the component part's fullpath based on the current load  
        options (if it can be found via those load options) whether it is currently  
        loaded or not.  If the component cannot be found only the leaf name is  
        returned.  
    */  
        UF_CALL(UF_ASSEM_ask_component_data(component, partname, refset_name,  
            instance_name, origin, csys_matrix, transform));  
    }

```

#### 代码解析

> 这段代码是用于NX二次开发的C++函数，其主要功能是获取NX组件的名称。具体来说，它实现了以下功能：
>
> 1. 函数名为ask_component_partname，接受一个组件的tag_t类型的参数component，以及一个用于存储组件名称的字符数组partname。
> 2. 定义了一些变量，包括实例名称、参考集名称、原点坐标、坐标系矩阵、变换矩阵等。
> 3. 调用UF_ASSEM_ask_component_data函数，该函数用于获取组件的相关信息，包括组件名称、参考集名称、实例名称、原点坐标、坐标系矩阵、变换矩阵等。
> 4. UF_ASSEM_ask_component_data函数的参数中，component表示组件的tag，partname用于存储组件名称，refset_name用于存储参考集名称，instance_name用于存储实例名称，origin用于存储原点坐标，csys_matrix用于存储坐标系矩阵，transform用于存储变换矩阵。
> 5. 如果组件在当前加载选项下能够找到，则返回组件的全路径名称；如果找不到，则只返回组件的叶子名称。
> 6. 函数通过UF_CALL宏来调用UF_ASSEM_ask_component_data函数，该宏用于处理函数调用的返回值，如果失败则记录错误。
>
> 综上所述，这段代码实现了查询NX组件名称的功能，通过UF_ASSEM_ask_component_data函数获取组件的详细信息，并使用partname参数返回组件名称。
>
