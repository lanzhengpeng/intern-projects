### 【3077】ask component shortname 根据组件查询简称

#### 代码

```cpp
    static void ask_component_partname(tag_t component, char *partname)  
    {  
        double  
            csys_matrix[9],  
            origin[3],  
            transform[4][4];  
        char  
            instance_name[UF_CFI_MAX_FILE_NAME_SIZE],  
            refset_name[MAX_ENTITY_NAME_SIZE+1];  
    /* This returns the component part's fullpath based on the current load   
        options whether it is loaded or not.  
    */  
        UF_CALL(UF_ASSEM_ask_component_data(component, partname, refset_name,  
            instance_name, origin, csys_matrix, transform));  
    }  
    static void ask_component_shortname(tag_t component, char *shortname)  
    {  
        logical  
            nxman;  
        char  
            partname[UF_CFI_MAX_PATH_NAME_SIZE];  
        ask_component_partname(component, partname);  
        UF_CALL(UF_is_ugmanager_active(&nxman));  
        if (nxman)  
            UF_CALL(UF_PART_name_for_display(partname, shortname));  
        else  
            UF_CALL(uc4574(partname, 2, shortname));  
    }

```

#### 代码解析

> 这段代码是用于NX二次开发，包含两个函数：
>
> 1. ask_component_partname: 该函数用于获取组件的完整路径名称。它接收组件的tag_t作为输入，然后调用UF_ASSEM_ask_component_data函数，以获取组件的完整路径名称，并将其存储在partname数组中。
> 2. ask_component_shortname: 该函数用于获取组件的短名称。它首先调用ask_component_partname获取组件的完整路径名称，然后检查NX Manager是否激活。如果激活，则调用UF_PART_name_for_display函数以获取短名称；如果不激活，则调用uc4574函数以获取短名称。
>
> 这段代码的主要作用是获取NX组件的名称信息，包括完整路径名称和短名称。这些信息可以用于显示组件信息，或作为后续操作的输入参数。
>
