### 【1104】display temporary facet 显示临时面

#### 代码

```cpp
    static int allocate_memory(unsigned int nbytes, void **where)  
    {  
        int  
            resp;  
        *where = UF_allocate_memory(nbytes, &resp);  
        return resp;  
    }  
    static void display_temporary_line(double start[3], double end[3])  
    {  
        UF_OBJ_disp_props_t  
            props = { 1, UF_OBJ_WHITE, UF_OBJ_NOT_BLANKED, UF_OBJ_WIDTH_NORMAL,  
                UF_OBJ_FONT_SOLID, FALSE};  
        UF_CALL(UF_DISP_display_temporary_line(NULL_TAG, UF_DISP_USE_ACTIVE_PLUS,  
            start, end, &props));  
    }  
    static void display_temporary_facet(tag_t model, int index)  
    {  
        int  
            jj,  
            n_facets,  
            n_verts;  
        double  
            (*verts)[3];  
        UF_CALL(UF_FACET_ask_max_facet_verts(model, &n_verts));  
        UF_CALL(allocate_memory(n_verts * 3 * sizeof(double), (void *)&verts));  
        UF_CALL(UF_FACET_ask_vertices_of_facet(model, index, &n_verts, verts));  
        for (jj = 0; jj < (n_verts - 1); jj++)  
            display_temporary_line(verts[jj], verts[jj+1]);  
        display_temporary_line(verts[n_verts-1], verts[0]);  
        UF_free(verts);  
    }

```

#### 代码解析

> 这段代码是用于NX Open C++ API进行二次开发的示例代码，主要实现了以下功能：
>
> 1. 内存分配函数allocate_memory：用于分配指定大小的内存空间，并返回指向分配内存的指针。
> 2. 临时线显示函数display_temporary_line：用于在NX中显示一条临时线段，可以设置线段的颜色、线宽等属性。
> 3. 临时面显示函数display_temporary_facet：用于在NX中显示一个临时面，首先获取面的顶点坐标，然后依次显示顶点之间的临时线段。
> 4. 在display_temporary_facet函数中，首先使用UF_FACET_ask_max_facet_verts获取面的最大顶点数，然后使用allocate_memory分配内存来存储顶点坐标。
> 5. 接着使用UF_FACET_ask_vertices_of_facet获取面的顶点坐标。
> 6. 遍历顶点，使用display_temporary_line显示每两个顶点之间的临时线段，最后显示最后一个顶点和第一个顶点之间的线段。
> 7. 显示完成后，使用UF_free释放分配的内存。
> 8. 这段代码通过NX Open C++ API实现了一些简单的二次开发功能，展示了NX二次开发的基本流程。
>
