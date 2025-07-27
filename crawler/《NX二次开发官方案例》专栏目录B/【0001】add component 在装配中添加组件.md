### 【0001】add component 在装配中添加组件

#### 代码

```cpp
    static void ask_wcs_info(double origin[3], double axes[9])  
    {  
        tag_t  
            wcs,  
            wcs_mx;  
        UF_CALL(UF_CSYS_ask_wcs(&wcs));  
        UF_CALL(UF_CSYS_ask_csys_info(wcs, &wcs_mx, origin));  
        UF_CALL(UF_CSYS_ask_matrix_values(wcs_mx, axes));  
    }  
    static void report_load_status(UF_PART_load_status_t *status)  
    {  
        char  
            msg[133];  
        int  
            ii;  
        for (ii=0; ii<status->n_parts; ii++)  
        {  
            UF_get_fail_message(status->statuses[ii], msg);  
            printf("    %s - %s\n", status->file_names[ii], msg);  
        }  
        if (status->n_parts > 0)  
        {  
            UF_free(status->statuses);  
            UF_free_string_array(status->n_parts, status->file_names);  
        }  
    }  
    static tag_t add_component(char *fspec)  
    {  
        tag_t  
            instance = NULL_TAG,  
            part = UF_PART_ask_display_part();  
        double  
            origin[3],  
            matrix[9];  
        UF_PART_load_status_t  
            error_status;  
        ask_wcs_info(origin, matrix);  
        UF_CALL(UF_ASSEM_add_part_to_assembly(part, fspec, NULL, NULL,  
            origin, matrix, 0, &instance, &error_status));  
        if (error_status.n_parts > 0) report_load_status(&error_status);  
        return (UF_ASSEM_ask_part_occ_of_inst(UF_ASSEM_ask_root_part_occ(part),  
            instance));  
    }

```

#### 代码解析

> 这段代码是用于NX CAD软件的二次开发，主要实现以下功能：
>
> 1. 获取当前坐标系信息：使用ask_wcs_info函数获取当前工作坐标系的原点和方向矩阵。
> 2. 报告加载状态：通过report_load_status函数，报告模型文件加载过程中的错误信息。
> 3. 添加组件：add_component函数用于将指定文件名的模型文件作为组件添加到当前装配体中。它获取当前坐标系信息，使用UF_ASSEM_add_part_to_assembly函数将文件作为组件添加到装配体中，并返回组件实例的标签。
> 4. 错误处理：在添加组件时，函数会检查加载状态，如果存在错误，则调用report_load_status函数报告错误信息。
>
> 总的来说，这段代码实现了在NX中通过二次开发，使用当前坐标系信息添加模型文件作为组件，并处理加载过程中的错误的功能。
>
