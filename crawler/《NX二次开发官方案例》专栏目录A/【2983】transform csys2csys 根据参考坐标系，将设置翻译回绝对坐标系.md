### 【2983】transform csys2csys 根据参考坐标系，将设置翻译回绝对坐标系

#### 代码

```cpp
static void transform_csys2csys(tag_t ref_csys, tag_t dest_csys, double mx[16])
{
    double
        csys[9],
        orig[3],
        tx1[16],
        tx2[16],
        tx3[16],
        tx4[16],
        v[3];
    int
        ii;
    tag_t
        csys_mx;
    UF_CALL(UF_CSYS_ask_csys_info(ref_csys, &csys_mx, orig));
    UF_CALL(UF_CSYS_ask_matrix_values(csys_mx, csys));
    tx1[0] = csys[0];
    tx1[1] = csys[1];
    tx1[2] = csys[2];
    tx1[3] = 0;
    tx1[4] = csys[3];
    tx1[5] = csys[4];
    tx1[6] = csys[5];
    tx1[7] = 0;
    tx1[8] = csys[6];
    tx1[9] = csys[7];
    tx1[10] = csys[8];
    tx1[11] = 0;
    /* set up to translate from reference csys back to absolute 译:根据参考坐标系，将设置翻译回绝对坐标系。 */
    for (ii=0; ii<3; ii++) v[ii] = -orig[ii];
    FTN(uf5943)(v, tx2);
    /* combine this with the rotation matrix from the reference csys 译:将这个与参考坐标系的旋转矩阵结合。 */
    FTN(uf5942)(tx2, tx1, tx3);
    UF_CALL(UF_CSYS_ask_csys_info(dest_csys, &csys_mx, orig));
    UF_CALL(UF_CSYS_ask_matrix_values(csys_mx, csys));
    /* Invert the rotation from the destination matrix 译:将目标矩阵的旋转进行反转。 */
    tx2[0] = csys[0];
    tx2[1] = csys[3];
    tx2[2] = csys[6];
    tx2[3] = 0;
    tx2[4] = csys[1];
    tx2[5] = csys[4];
    tx2[6] = csys[7];
    tx2[7] = 0;
    tx2[8] = csys[2];
    tx2[9] = csys[5];
    tx2[10] = csys[8];
    tx2[11] = 0;
    /* set up to translate from abs to the destination csys 译:设置从ABS坐标系转换到目标坐标系。 */
    FTN(uf5943)(orig, tx1);
    /* combine this with the inverted rotation csys above 译:结合上述反转旋转坐标系。 */
    FTN(uf5942)(tx2, tx1, tx4);
    /* and the one from the reference csys 译:根据上下文，这句话的翻译是：

“并且来自参考坐标系的那一个。” */
    FTN(uf5942)(tx3, tx4, mx);
}

```

#### 代码解析

> 这是段用于NX（NX Nastran）的二次开发代码，其主要功能是将一个坐标系（ref_csys）转换到另一个坐标系（dest_csys），并将转换矩阵存储在mx中。
>
> 代码流程如下：
>
> 1. 获取参考坐标系（ref_csys）的旋转矩阵和原点坐标，并构建一个4x4的旋转矩阵tx1。
> 2. 构建一个4x4的平移矩阵tx2，用于将参考坐标系平移回绝对坐标系。
> 3. 将tx2和tx1相乘，得到从参考坐标系到绝对坐标系的转换矩阵tx3。
> 4. 获取目标坐标系（dest_csys）的旋转矩阵和原点坐标，并构建一个反转的4x4旋转矩阵tx2。
> 5. 构建一个4x4的平移矩阵tx1，用于将绝对坐标系平移到目标坐标系。
> 6. 将tx2和tx1相乘，得到从绝对坐标系到目标坐标系的转换矩阵tx4。
> 7. 将tx3和tx4相乘，得到最终的转换矩阵mx，存储在mx中。
> 8. 最后，mx就是将参考坐标系（ref_csys）转换到目标坐标系（dest_csys）的4x4转换矩阵。
>
> 这段代码通过坐标系之间的旋转和平移，实现了坐标系之间的转换，是NX二次开发中常用的一种坐标变换实现方式。
>
