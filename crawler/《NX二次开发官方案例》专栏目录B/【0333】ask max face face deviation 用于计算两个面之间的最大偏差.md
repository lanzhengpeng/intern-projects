### 【0333】ask max face face deviation 用于计算两个面之间的最大偏差

#### 代码

```cpp
static void build_temp_filespec(char *fspec, int ftype)
{
    tag_t
        part = UF_PART_ask_display_part();
    char
        dspec[UF_CFI_MAX_PATH_NAME_SIZE],
        part_name[UF_CFI_MAX_FILE_NAME_SIZE],
        part_fspec[UF_CFI_MAX_PATH_NAME_SIZE],
        *tmp_dir;
    UF_CALL(UF_translate_variable("UGII_TMP_DIR", &tmp_dir));
    UF_PART_ask_part_name(part, part_fspec);
    UF_CALL(uc4576(part_fspec, 2, dspec, part_name));
    UF_CALL(uc4578(part_name, 2, part_name));
    UF_CALL(uc4575(tmp_dir, ftype, part_name, fspec));
}
/*  This function demonstrates using the GRIP command DEVCHK to do
    face to face deviation checking from an API program.
    The code for the GRIP program called by this program is:
        ENTITY/ one_face, two_face
        NUMBER/ u_pts, v_pts, d_tol, a_tol
        UFARGS/ one_face, two_face, u_pts, v_pts, d_tol, a_tol
        DRAW/OFF
        DEVCHK/ one_face, TO, two_face, u_pts, v_pts, TOLER, d_tol, a_tol
        DRAW/ON
        HALT
*/
static double ask_max_face_face_deviation(
    tag_t face1,
    tag_t face2,
    int u_check_points,
    int v_check_points,
    double distance_tolerance,
    double angle_tolerance)
{
    double
        max_dev = -1,
        U_check = u_check_points,
        V_check = v_check_points;
    char
        a_line[80],
        tfn[UF_CFI_MAX_PATH_NAME_SIZE];
    UF_args_t
        args[6];
    FILE
        *data_file;
    args[0].type = UF_TYPE_TAG_T;
    args[0].length = 0;
    args[0].address = &face1;
    args[1].type = UF_TYPE_TAG_T;
    args[1].length = 0;
    args[1].address = &face2;
    args[2].type = UF_TYPE_DOUBLE;
    args[2].length = 0;
    args[2].address = &U_check;
    args[3].type = UF_TYPE_DOUBLE;
    args[3].length = 0;
    args[3].address = &V_check;
    args[4].type = UF_TYPE_DOUBLE;
    args[4].length = 0;
    args[4].address = &distance_tolerance;
    args[5].type = UF_TYPE_DOUBLE;
    args[5].length = 0;
    args[5].address = &angle_tolerance;
    build_temp_filespec(tfn, 4);
    UF_CALL(uc4400(4, tfn, 0));
    UF_CALL(UF_call_grip("grip/devchk.grx", 6, args));
    UF_CALL(uc4400(0, tfn, 0));
    if ((data_file = fopen(tfn, "r")) != NULL)
    {
        while (!feof(data_file))
        {
            fgets(a_line, 80, data_file);
            if (strstr(a_line, "Maximum Distance Error"))
            {
                sscanf(a_line, "Maximum Distance Error   = %lf", &max_dev);
                break;
            }
        }
        fclose(data_file);
    }
    UF_CALL(uc4561(tfn, 4));
    return max_dev;
}

```

#### 代码解析

> 这段NX二次开发代码的主要功能是计算两个面之间的最大偏差值。
>
> 关键点包括：
>
> 1. 定义了build_temp_filespec函数，用于生成临时文件路径。
> 2. ask_max_face_face_deviation函数用于计算两个面之间的最大偏差。
> 3. 该函数首先调用build_temp_filespec生成临时文件路径。
> 4. 然后调用UF_call_grip执行GRIP程序devchk.grx，传递两个面、采样点数、距离公差、角度公差等参数。
> 5. GRIP程序devchk.grx执行面面偏差检查，并将最大偏差值写入临时文件。
> 6. ask_max_face_face_deviation函数读取临时文件，获取最大偏差值。
> 7. 最后删除临时文件并返回最大偏差值。
>
> 通过GRIP程序的调用，实现了NX的二次开发，实现了面面偏差检查的功能。
>
