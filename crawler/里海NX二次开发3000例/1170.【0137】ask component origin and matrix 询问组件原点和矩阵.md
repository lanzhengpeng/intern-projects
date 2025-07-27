### 【0137】ask component origin and matrix 询问组件原点和矩阵

#### 代码

```cpp
    static void ask_component_origin_and_matrix(tag_t component, double origin[3],  
        double csys_matrix[9])  
    {  
        double  
            transform[4][4];  
        char  
            instance_name[UF_CFI_MAX_FILE_NAME_SIZE],  
            partname[UF_CFI_MAX_PATH_NAME_SIZE],  
            refset_name[MAX_ENTITY_NAME_SIZE+1];  
        UF_CALL(UF_ASSEM_ask_component_data(component, partname, refset_name,  
            instance_name, origin, csys_matrix, transform));  
    }

```

#### 代码解析

> 这段代码是NX Open C++ API中的函数，用于查询组件的原始位置和坐标系矩阵。
>
> 主要功能如下：
>
> 1. 获取组件的名称、部件名称、引用集名称和实例名称。
> 2. 获取组件的原始位置坐标(origin[3])。
> 3. 获取组件的坐标系矩阵(csys_matrix[9])。
> 4. 获取组件的变换矩阵(transform[4][4])。
>
> 这个函数通过调用UF_ASSEM_ask_component_data()函数来实现上述功能。该函数需要传入组件的tag_t类型的标识，以及其他用于接收组件名称、位置和矩阵信息的参数。通过这个函数，我们可以获取组件的详细信息，用于后续的装配或其他操作。
>
