### 【2838】set wcs to current view lower left corner 将当前视图左下角设置为WCS原点

#### 代码

```cpp
    static void map_abs2view(double c[3], tag_t view)  
    {  
        char  
            vname[30];  
        double  
            abs[9] = { 0,0,0, 1,0,0, 0,1,0 },  
            vw[12] = { 0,0,0 },  
            mx[12];  
        int  
            irc;  
        UF_CALL(UF_OBJ_ask_name(view, vname));  
        UF_CALL(uc6433(vname, &vw[3]));  
        FTN(uf5940)(abs, vw, mx, &irc);  
        FTN(uf5941)(c, mx);  
    }  
    static void map_view2abs(double c[3], tag_t view)  
    {  
        char  
            vname[30];  
        double  
            abs[9] = { 0,0,0, 1,0,0, 0,1,0 },  
            vw[12] = { 0,0,0 },  
            mx[12];  
        int  
            irc;  
        UF_CALL(UF_OBJ_ask_name(view, vname));  
        UF_CALL(uc6433(vname, &vw[3]));  
        FTN(uf5940)(vw, abs, mx, &irc);  
        FTN(uf5941)(c, mx);  
    }  
    static void set_wcs_to_current_view_lower_left_corner()  
    {  
        tag_t  
            csys,                       /* Csys object  译:Csys object 是一个用于描述坐标系的对象，通常用于CAD和CAM系统中。其主要功能包括：

1. 描述坐标系的原点位置，即坐标系的原点在三维空间中的坐标值。
2. 描述坐标系的旋转角度，即坐标系相对于标准坐标系（通常为笛卡尔坐标系）的旋转角度。
3. 描述坐标系的缩放比例，即坐标系中单位长度与标准坐标系中单位长度的比值。
4. 描述坐标系的镜像变换，即坐标系相对于标准坐标系的镜像变换情况。

Csys object 通过这些参数，能够完整描述一个坐标系在三维空间中的位置和方向，是CAD和CAM系统中进行图形变换和坐标转换的重要基础。 */  
            matobj,                     /* Matrix object 译:翻译如下：

Matrix对象 */  
            wv;  
        double  
            bounds[4],  
            origin[3],                  /* View's center 译:View的中心 */  
            matrix[9],                  /* View's matrix 译:视图的矩阵 */  
            scale;  
        UF_CALL(uc6430("", origin, &scale));  
        UF_CALL(uc6433("", matrix));  
        UF_CALL(UF_VIEW_ask_work_view(&wv));  
        UF_CALL(UF_VIEW_ask_current_xy_clip(wv, bounds));  
        map_abs2view(origin, wv);  
        origin[0] = origin[0] + bounds[0];  
        origin[1] = origin[1] + bounds[2];  
        origin[2] = 0;  
        map_view2abs(origin, wv);  
        if (!UF_CALL(UF_CSYS_create_matrix(matrix, &matobj)) &&  
            !UF_CALL(UF_CSYS_create_temp_csys(origin, matobj, &csys)))  
            UF_CALL(UF_CSYS_set_wcs(csys));  
    }

```

#### 代码解析

> 这段代码是用于NX的二次开发，主要实现了以下功能：
>
> 1. 视图坐标转换：通过map_abs2view和map_view2abs函数，实现了视图坐标系和绝对坐标系之间的转换。这些函数接收视图名称和三维坐标，通过调用内部函数计算坐标转换矩阵，然后将坐标转换到另一个坐标系。
> 2. 设置工作坐标系(WCS)：set_wcs_to_current_view_lower_left_corner函数首先获取当前视图的中心和矩阵，然后计算视图的左下角在绝对坐标系下的坐标，最后创建一个临时坐标系并将左下角设置为原点，从而将WCS设置到当前视图的左下角。
> 3. 坐标转换：内部函数uf5940和uf5941实现了坐标转换矩阵的生成和坐标转换功能。这些函数为Fortran语言编写的，需要通过C++调用Fortran接口。
> 4. 视图信息获取：代码中使用了多个NX提供的API函数，如UF_OBJ_ask_name、UF_VIEW_ask_work_view、UF_VIEW_ask_current_xy_clip等，以获取视图名称、获取当前视图、获取视图的裁剪区域等信息。
> 5. 坐标系操作：代码使用了NX提供的坐标系API函数，如UF_CSYS_create_matrix、UF_CSYS_create_temp_csys、UF_CSYS_set_wcs等，以创建坐标系、设置坐标系原点等。
>
> 总体而言，这段代码通过调用NX提供的API函数，实现了视图坐标系与绝对坐标系之间的转换，以及设置工作坐标系到视图左下角的功能。
>
