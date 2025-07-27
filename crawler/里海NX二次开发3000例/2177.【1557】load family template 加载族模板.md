### 【1557】load family template 加载族模板

#### 代码

```cpp
    static void load_family_template(tag_t family)  
    {  
        double  
            csys_matrix[9],  
            origin[3],  
            transform[4][4];  
        char  
            instance_name[UF_CFI_MAX_FILE_NAME_SIZE],  
            refset_name[MAX_ENTITY_NAME_SIZE+1],  
            template_part[MAX_FSPEC_SIZE+1];  
        UF_print_syslog("\n!!! Please ignore error messages between here - \n\n",  
            FALSE);  
    /*  This will fail and write an error to the syslog but will perform  
        a load on demand on the template part  
    */  
        UF_ASSEM_ask_component_data(family, template_part, refset_name,  
                instance_name, origin, csys_matrix, transform);  
        UF_print_syslog("\n!!! - and here!\n\n", FALSE);  
    }

```

#### 代码解析

> 这段代码是NX Open C++的二次开发示例，主要实现了加载NX部件模板的功能。
>
> 代码的主要步骤包括：
>
> 1. 定义了各种所需的变量，包括矩阵、坐标、名称等。
> 2. 使用UF_print_syslog打印提示信息，提示忽略错误信息。
> 3. 调用UF_ASSEM_ask_component_data函数，尝试获取部件模板的详细信息。这个函数会失败并打印错误信息到系统日志，但会触发NX的“按需加载”功能，从而加载模板部件。
> 4. 再次使用UF_print_syslog打印提示信息，表示错误信息结束。
>
> 这段代码的目的是在NX中加载一个部件模板，而无需关心模板文件的实际路径。通过UF_ASSEM_ask_component_data的“按需加载”功能，可以实现加载模板文件的目的。虽然这个函数会失败并打印错误，但它触发了NX的“按需加载”机制，从而实现了加载模板文件的目的。
>
