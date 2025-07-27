### 【0931】create surf from cloud 创建云点曲面

#### 代码

```cpp
    /*HEAD CREATE_SURF_FROM_CLOUD CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_modl.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog是V18版本新增的函数。 */  
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
    static void report_tag_type_and_subtype(char *name, tag_t object)  
    {  
        int  
            subtype,  
            type;  
        if (object != NULL_TAG)  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(object, &type, &subtype));  
            printf("%s = %d, type = %d, subtype = %d\n", name,object,type,subtype);  
        }  
        else printf("%s = %d (NULL_TAG)\n", name, object);  
    }  
    static tag_t ask_sheet_face(tag_t sheet)  
    {  
        int  
            n;  
        tag_t  
            face = NULL_TAG;  
        uf_list_p_t  
            face_list;  
        UF_CALL(UF_MODL_ask_body_faces(sheet, &face_list));  
        UF_CALL(UF_MODL_ask_list_count(face_list, &n));  
        if (n == 1) UF_CALL(UF_MODL_ask_list_item(face_list, 0, &face));  
        UF_CALL(UF_MODL_delete_list(&face_list));  
        return face;  
    }  
    static void do_it(void)  
    {  
        int  
            max_err_index;  
        tag_t  
            face,  
            surface_tag = NULL_TAG;  
        double  
            average_error,  
            cloud_pnts[16][3]=  
            { { 2200.0,  548.0, 1472.0 }, { 1400.0, 1168.0, 1213.0 },  
              { 1000.0,  285.0, 1377.0 }, { 1000.0, 1107.0, 1154.0 },  
              { 1000.0, -254.0,  950.0 }, { 2200.0,  951.0, 1462.0 },  
              { 1400.0, -353.0, 1049.0 }, { 1800.0,  605.0, 1388.0 },  
              { 1400.0,  222.0, 1550.0 }, { 1800.0, -426.0, 1121.0 },  
              { 2200.0,  207.0, 1750.0 }, { 1800.0, 1007.0, 1406.0 },  
              { 1800.0,  259.0, 1655.0 }, { 1400.0,  649.0, 1301.0 },  
              { 2200.0, -479.0, 1175.0 }, { 1000.0,  704.0, 1167.0 } },  
            max_error,  
            mtx[2][3] = { {1.0, 0.0, 0.0}, {0.0, 1.0, 0.0} };  
        UF_CALL(UF_MODL_create_surf_from_cloud( 16, cloud_pnts, mtx, NULL, 3, 3,  
            1, 1, 0, &average_error, &max_error, &max_err_index, &surface_tag ));  
        report_tag_type_and_subtype("surface_tag", surface_tag);  
        face = ask_sheet_face(surface_tag);  
        report_tag_type_and_subtype("face", face);  
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

> 这段代码是一个NX二次开发示例，主要功能是从点云创建曲面。以下是代码的主要内容和流程：
>
> 1. 头文件包含：代码包含了必要的NX Open头文件，包括uf.h, uf_ui.h等。
> 2. 错误处理宏：定义了一个错误处理宏UF_CALL，用于捕捉函数调用错误并打印错误信息。
> 3. 错误报告函数：定义了一个错误报告函数report_error，用于打印函数调用失败时的错误信息。
> 4. 标签类型和子类型报告函数：定义了一个函数report_tag_type_and_subtype，用于打印对象标签的类型和子类型。
> 5. 询问面函数：定义了一个函数ask_sheet_face，用于询问一个sheet对象的面标签。
> 6. 主要执行函数：定义了一个函数do_it，其中：定义了一个点云数组cloud_pnts，包含16个点的坐标。调用UF_MODL_create_surf_from_cloud函数，使用点云数据创建曲面。打印创建的曲面和面的标签类型和子类型。
> 7. 定义了一个点云数组cloud_pnts，包含16个点的坐标。
> 8. 调用UF_MODL_create_surf_from_cloud函数，使用点云数据创建曲面。
> 9. 打印创建的曲面和面的标签类型和子类型。
> 10. UFusr函数：定义了UFusr函数，这是NX二次开发的入口函数。在该函数中初始化NX Open，调用do_it函数执行主要功能，然后终止NX Open。
> 11. 卸载函数：定义了一个卸载函数ufusr_ask_unload，用于返回立即卸载的标志。
>
> 总体来说，这段代码实现了从点云数据创建曲面的功能，并包含了错误处理和类型报告等辅助功能。
>
