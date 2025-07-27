### 【0667】create display mesh 创建显示网格

#### 代码

```cpp
    /*HEAD CREATE_DISPLAY_MESH CCC UFUN */  
    /*  This program does not work in NX 4 - see PR 5552540 译:这个程序在NX 4中无法工作 - 请参阅 PR 5552540。 */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_sf.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档，翻译如下：

注意：UF_print_syslog是V18版本新增的函数。 */  
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
            node_ids[4] = { 1, 2, 3, 4 },  
            elem_ids[1] = { 1 },  
            *elem_node_ids[1];  
        tag_t  
            mesh_tag;  
        double  
            corners[4][3] = { {0,0,0}, {10,0,0}, {10,20,0}, {0,20,0} },  
            (* node_coords)[3];  
        UF_SF_disp_mesh_t  
            disp_mesh = { UF_SF_DIMENSION_2D, 1, 4, 4, NULL, NULL, NULL, NULL };  
        node_coords = corners;  
        elem_node_ids[0] = node_ids;  
        disp_mesh.node_coords = node_coords;  
        disp_mesh.node_ids = node_ids;  
        disp_mesh.elem_ids = elem_ids;  
        disp_mesh.elem_node_ids = elem_node_ids;  
        if (UF_CALL(UF_SF_create_disp_mesh(&disp_mesh, &mesh_tag))) return;  
        UF_CALL(UF_SF_display_mesh(mesh_tag));  
        uc1601("Mesh displayed", TRUE);  
        UF_CALL(UF_SF_delete_disp_mesh(1, &mesh_tag));  
    }  
    /* 里海 */  
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

> 这是段NX二次开发代码，用于创建和显示一个二维显示网格，主要功能和步骤如下：
>
> 1. 错误报告函数：定义了一个错误报告函数report_error，用于在函数调用失败时输出错误信息。
> 2. 创建显示网格：定义了函数do_it，用于创建一个二维显示网格，包括节点坐标、节点ID、元素ID和元素节点ID，然后调用UF_SF_create_disp_mesh函数创建显示网格，并获取网格的标签。
> 3. 显示网格：调用UF_SF_display_mesh函数显示创建的网格。
> 4. 删除网格：调用UF_SF_delete_disp_mesh函数删除创建的网格。
> 5. 用户函数：定义了ufusr函数作为NX用户函数的入口点，在该函数中初始化NX系统，调用do_it函数创建和显示网格，然后终止NX系统。
> 6. 卸载函数：定义了ufusr_ask_unload函数，返回立即卸载标志，表示该库可以立即卸载。
>
> 通过这些步骤，该代码实现了创建和显示一个简单的二维显示网格的功能，展示了NX二次开发的基本流程。
>
