### 【2844】set wcs while in context 在上下文中设置WCS

#### 代码

```cpp
    /*HEAD SET_WCS_WHILE_IN_CONTEXT CCC UFUN */  
    /* This sample program was created as a workaround for PR 672287 里海译:这个示例程序是为PR 672287创建的变通方法。 */  
    /* It is currenlty designed to be call from grip which passes the 里海译:目前正在设计从grip调用它，传递参数。 */  
    /* tag of a csys ent. It will map the origin and matrix of the entity 里海译:翻译为：将实体csys的标签进行映射，包括实体的原点和矩阵。 */  
    /* and create the csys accordingly                                里海译:根据上下文，翻译为：相应地创建坐标系。 */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_trns.h>  
    #include <uf_mtx.h>  
    #include <uf_curve.h>  
    #include <uf_assem.h>  
    #include <uf_disp.h>  
    #include <uf_csys.h>  
    #include <uf_ui.h>  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133],  
                 msg[133];  
            sprintf(msg, "*** ERROR code %d at line %d in %s:\n+++ ",  
                irc, line, file);  
            UF_get_fail_message(irc, err);  
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog 是 V18 中新增的函数。 */  
            UF_print_syslog(msg, FALSE);  
            UF_print_syslog(err, FALSE);  
            UF_print_syslog("\n", FALSE);  
            UF_print_syslog(call, FALSE);  
            UF_print_syslog(";\n", FALSE);  
            if (!UF_UI_open_listing_window())  
            {  
                UF_UI_write_listing_window(msg);  
                UF_UI_write_listing_window(err);  
                UF_UI_write_listing_window("\n");  
                UF_UI_write_listing_window(call);  
                UF_UI_write_listing_window(";\n");  
            }  
        }  
        return(irc);  
    }  
    static void do_it(void)  
    {  
        int  
            ret,  
            ii;  
        double  
            new_mat[12],  
            csys_new[3],  
            csys_orig[3],  
            comp_orig[3],  
            csys_mat[9],  
            tr_mat[4][4],  
            matrix_val[9];  
        tag_t  
            pt_tag,  
            wcs_to,  
            new_matrix,  
            matrix_id,  
            part_occ,  
            csys_ent;  
        UF_args_t  
            grip_arg_list;  
        tag_t  work_part_tag, work_part_occ, root_part_occ, root_part_tag;  
        tag_t  work_matrix_tag, root_matrix_tag;  
        double work_matrix_values[9], work_origin[3];  
        double root_matrix_values[9], root_origin[3];  
        double comp_origin[3], comp_csys_matrix[9], comp_transform_matrix[4][4];  
        double comp_matrix_transpose[9];  
        char   part_name[MAX_FSPEC_SIZE+1];  
        char   refset_name[MAX_FSPEC_SIZE+1];  
        char   instance_name[MAX_FSPEC_SIZE+1];  
        tag_t  root_csys_tag;  
        int    i;  
        grip_arg_list.type    = UF_TYPE_TAG_T;  
        grip_arg_list.length  = 1;  
        grip_arg_list.address = &csys_ent;  
        UF_CALL(UF_ask_grip_args(1, &grip_arg_list));  
        printf(" ====== Data received from grip =========\n");  
        /* Get the origin and matrix from the csys passed in. 里海译:从传入的坐标系统中获取原点和矩阵。 */  
        UF_CSYS_ask_csys_info( csys_ent, &work_matrix_tag, work_origin );  
        UF_CSYS_ask_matrix_values( work_matrix_tag, work_matrix_values );  
        /* Get the origin and  matrix for the component's tranformation. 里海译:获取组件的变换的原始和矩阵。 */  
        work_part_occ = UF_ASSEM_ask_work_occurrence();  
        UF_ASSEM_ask_component_data( work_part_occ, part_name, refset_name,  
                                     instance_name, comp_origin,  
                                     comp_csys_matrix, comp_transform_matrix );  
        /* Change the work part up to the assembly 里海译:将工作部分更改到装配阶段。 */  
        work_part_tag = UF_ASSEM_ask_work_part();  
        work_part_occ = UF_ASSEM_ask_work_occurrence();  
        root_part_occ = UF_ASSEM_ask_part_occurrence( work_part_occ );  
        root_part_tag = UF_ASSEM_ask_prototype_of_occ( root_part_occ );  
        if ( root_part_tag != NULL )  
           UF_ASSEM_set_work_part( root_part_tag );  
        /* Transform the csys's matrix by the component's transformation matrix,  
           creating a new "root" matrix.   Same for the origin. */  
        UF_MTX4_vec3_multiply( work_origin, (double *)comp_transform_matrix, root_origin );  
        UF_MTX3_multiply( comp_csys_matrix, work_matrix_values, root_matrix_values );  
        /* Make a new csys with the root origin and matrix 里海译:创建一个新的坐标系，以原点为根，并具有矩阵。 */  
        UF_CSYS_create_matrix(  root_matrix_values, &root_matrix_tag );  
        UF_CSYS_create_csys( root_origin, root_matrix_tag, &root_csys_tag );  
        UF_CALL(UF_CSYS_set_wcs(root_csys_tag));  
        UF_ASSEM_set_work_part( work_part_tag );  
    }  
    void ufusr(char *param, int *retcode, int paramLen)  
    {  
        if (UF_CALL(UF_initialize())) return;  
        do_it();  
        UF_terminate();  
    }  
    int ufusr_ask_unload(void)  
    {  
        return (UF_UNLOAD_IMMEDIATELY);  
    }

```

#### 代码解析

> 这段NX二次开发代码的主要功能是：
>
> 1. 从grip中接收坐标系csys的tag，并获取该csys的原点和矩阵。
> 2. 获取当前工作部件的信息，包括部件名、引用集名、实例名，以及该部件的变换原点和变换矩阵。
> 3. 将工作部件切换到根部件。
> 4. 将csys的原点通过部件的变换矩阵变换，得到新的原点。
> 5. 将csys的矩阵与部件的变换矩阵相乘，得到新的矩阵。
> 6. 使用新的原点和矩阵创建一个新的坐标系，并设置为工作坐标系。
> 7. 最后将工作部件切换回原来的部件。
>
> 这段代码的主要目的是，在上下文中根据传入的csys信息创建一个新的坐标系，并将其设置为工作坐标系。这可以用于在装配中设置合适的坐标系。
>
> 代码中使用了NX提供的UF函数，例如UF_CSYS_ask_csys_info、UF_ASSEM_ask_work_occurrence等，以及一些矩阵运算函数，如UF_MTX4_vec3_multiply和UF_MTX3_multiply。
>
> 代码结构清晰，通过参数传递和函数调用实现了所需功能。
>
