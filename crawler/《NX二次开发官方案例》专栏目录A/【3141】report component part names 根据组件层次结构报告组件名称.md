### 【3141】report component part names 根据组件层次结构报告组件名称

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
    #define INDENT(X)   { int jj; for (jj = 0; jj < X; jj++) ECHO(" "); }  
    static void report_component_part_names(tag_t component, int level,  
        logical nxman)  
    {  
        int  
            ii,  
            n;  
        tag_t  
            *children;  
        char  
            partname[MAX_FSPEC_SIZE+1],  
            child_pn[MAX_FSPEC_SIZE+1];  
        ask_component_partname(component, partname);  
        UF_CALL(UF_PART_name_for_display(partname, child_pn));  
        INDENT(level);  
        ECHO("%s\n", child_pn);  
        n = UF_ASSEM_ask_all_part_occ_children(component, &children);  
        for (ii = 0; ii < n; ii++)  
            report_component_part_names(children[ii], level+1, nxman);  
        if (n > 0) UF_free(children);  
    }

```

#### 代码解析

> 这段代码是用于遍历NX模型中的组件层级关系，并打印出每个组件的名称。具体来说，代码的主要功能包括：
>
> 1. 定义了一个ask_component_partname函数，用于获取组件的名称，并填充到传入的partname字符串中。
> 2. 定义了一个report_component_part_names函数，用于递归遍历组件的子组件，打印每个组件的名称。它首先获取当前组件的名称，然后遍历其所有子组件，对每个子组件递归调用自身。
> 3. 在递归遍历的过程中，通过缩进表示层级关系。
> 4. 使用了NX提供的UF_ASSEM和UF_PART等相关函数，以获取组件名称和遍历组件关系。
> 5. 使用了宏INDENT来生成缩进字符串。
> 6. 通过UF_free释放了动态分配的内存。
>
> 总的来说，这段代码实现了NX模型组件树结构的递归遍历，并以树形格式打印出每个组件的名称，对于理解NX模型的组件结构非常有帮助。
>
