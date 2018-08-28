#include "sheet.h"
#include "..\Graphic\graphic.h"
#include "..\memory\memory.h"
#include <stdio.h>
#include <string.h>

void sheet(struct SHEET_INFO *sheet_info,struct MEMMAN *memman, unsigned char *vram, int xsize, int ysize){
    int mx, my;
    char s[40]="arafat";
    //sheet_info->buf_mouse=(unsigned char *)s;
    struct BUF_SHT *back,*mouse;
	back= &sheet_info->buf_sht[0];
	mouse= &sheet_info->buf_sht[1];

    sheet_info->shtctl=shtctl_init(memman, vram, xsize,ysize);

    back->sht= sheet_alloc(sheet_info->shtctl);

    //sheet_info->sht_mouse= sheet_alloc(sheet_info->shtctl);
	mouse->sht= sheet_alloc(sheet_info->shtctl);

    back->buf=(unsigned char *) memman_alloc_4K(memman, xsize * ysize);
	mouse->buf=(unsigned char *) memman_alloc(memman, sizeof(char)*256);

    sheet_setbuf(back->sht, back->buf, xsize, ysize, -1); /* �����F�Ȃ� */
	sheet_setbuf(mouse->sht, mouse->buf, 16, 16, 15);
    init_screen(back->buf, xsize, ysize);
    init_mouse_cursor8(mouse->buf, 15);
	sheet_slide(sheet_info->shtctl, back->sht, 0, 0);
	mx = (xsize - 16) / 2; /* ��ʒ����ɂȂ�悤�ɍ��W�v�Z */
	my = (ysize - 28 - 16) / 2;
	sheet_slide(sheet_info->shtctl, mouse->sht, mx, my);
	sheet_updown(sheet_info->shtctl, back->sht,  0);
	sheet_info->size=1;

}
void updown_mouse(struct SHEET_INFO *sheet_info){
	
	sheet_updown(sheet_info->shtctl, sheet_info->buf_sht[1].sht, sheet_info->size);
}

void Make_Window(struct SHEET_INFO *sheet_info,struct MEMMAN *memman,struct Window *window,int x0,int  y0){
	struct BUF_SHT *temp;
	temp=&sheet_info->buf_sht[++sheet_info->size];
	char s[20];
	temp->buf=(unsigned char*)memman_alloc_4K(memman,window->height*window->width);
	temp->sht=sheet_alloc(sheet_info->shtctl);
	sheet_setbuf(temp->sht,temp->buf,window->width,window->height,15);
	make_window8(temp->buf,window->width,window->height,window->title);
	temp->sht->shtctl=sheet_info->shtctl;
	//make_textbox8(temp->sht,208,28,100,20);
	sheet_slide(sheet_info->shtctl,temp->sht,x0,y0);
	sheet_updown(sheet_info->shtctl,temp->sht,sheet_info->size-1);
	
	strcpy(temp->name,window->title);
	window->sht=temp->sht;
}

void Make_Window_Base(struct SHEET_INFO *sheet_info,struct MEMMAN *memman,struct Window *window){
	struct BUF_SHT *temp;
	temp=&sheet_info->buf_sht[++sheet_info->size];
	char s[20];
	temp->buf=(unsigned char*)memman_alloc_4K(memman,window->height*window->width);
	temp->sht=sheet_alloc(sheet_info->shtctl);
	sheet_setbuf(temp->sht,temp->buf,window->width,window->height,15);
	make_window8(temp->buf,window->width,window->height,window->title);
	strcpy(temp->name,window->title);
	temp->sht->shtctl=sheet_info->shtctl;
	window->sht=temp->sht;
}

void Make_Console_Base(struct SHEET_INFO *sheet_info,struct MEMMAN *memman,struct Window *window){
	struct BUF_SHT *temp;
	temp=&sheet_info->buf_sht[++sheet_info->size];
	char s[20];
	temp->buf=(unsigned char*)memman_alloc_4K(memman,window->height*window->width);
	temp->sht=sheet_alloc(sheet_info->shtctl);
	sheet_setbuf(temp->sht,temp->buf,window->width,window->height,15);
	make_Console8(temp->buf,window->width,window->height,window->title);
	//make_textbox8(temp->sht,8,28,100,20);
	strcpy(temp->name,window->title);
	temp->sht->shtctl=sheet_info->shtctl;
	window->sht=temp->sht;
}

void Make_Win1(struct SHEET_INFO *sheet_info,struct MEMMAN *memman,struct Window *window,int x0,int  y0){
	struct BUF_SHT *temp;
	temp=&sheet_info->buf_sht[++sheet_info->size];
	char s[20];
	temp->buf=(unsigned char*)memman_alloc_4K(memman,window->height*window->width);
	temp->sht=sheet_alloc(sheet_info->shtctl);
	sheet_setbuf(temp->sht,temp->buf,window->width,window->height,15);
	make_window8(temp->buf,window->width,window->height,window->title);
	make_textbox8(temp->sht,8,28,100,20);
	sheet_slide(sheet_info->shtctl,temp->sht,x0,y0);
	sheet_updown(sheet_info->shtctl,temp->sht,sheet_info->size-1);
	
	strcpy(temp->name,window->title);
	window->sht=temp->sht;
	window->sht->shtctl=sheet_info->shtctl;
}


void Make_Console(struct SHEET_INFO *sheet_info,struct MEMMAN *memman,struct Window *window,int x0,int  y0){
	Make_Console_Base(sheet_info,memman,window);
	sheet_slide(sheet_info->shtctl,window->sht,x0,y0);
	sheet_updown(sheet_info->shtctl,window->sht,sheet_info->size-1);

}


struct SHTCTL *shtctl_init(struct MEMMAN *memman, unsigned char *vram, int xsize, int ysize)
{
	struct SHTCTL *ctl;
	int i;
	ctl = (struct SHTCTL *) memman_alloc_4K(memman, sizeof (struct SHTCTL));
	if (ctl == 0) {
		return ctl;
	}
	ctl->map=(unsigned char *) memman_alloc_4K(memman,xsize*ysize);
	if(ctl->map==0){
		memman_free_4K(memman,(int)ctl,sizeof(struct SHTCTL));
		return ctl;
	}
	ctl->vram = vram;
	ctl->xsize = xsize;
	ctl->ysize = ysize;
	ctl->top = -1; /* �V�[�g�͈ꖇ��Ȃ� */
	for (i = 0; i < MAX_SHEETS; i++) {
		ctl->sheets0[i].flags = 0; /* ���g�p�}�[�N */
	}
    return ctl;
}

struct SHEET *sheet_alloc(struct SHTCTL *ctl)
{
	struct SHEET *sht;
	int i;
	for (i = 0; i < MAX_SHEETS; i++) {
		if (ctl->sheets0[i].flags == 0) {
			sht = &ctl->sheets0[i];
			sht->flags = SHEET_USE; /* �g�p���}�[�N */
			sht->height = -1; /* ��\���� */
			sht->shtctl=ctl;
			sht->task=0;
			return sht;
		}
	}
	return 0;	/* �S�ẴV�[�g���g�p�������� */
}

void sheet_setbuf(struct SHEET *sht, unsigned char *buf, int xsize, int ysize, int col_inv)
{
	sht->buf = buf;
	sht->bxsize = xsize;
	sht->bysize = ysize;
	sht->col_inv = col_inv;
	return;
}

void sheet_updown(struct SHTCTL *ctl, struct SHEET *sht, int height)
{
	int h, old = sht->height; /* �ݒ�O�̍�����L������ */

	/* �w�肪�Ⴗ���⍂������������A�C������ */
	if (height > ctl->top + 1) {
		height = ctl->top + 1;
	}
	if (height < -1) {
		height = -1;
	}
	sht->height = height; /* ������ݒ� */

	/* �ȉ��͎��sheets[]�̕��בւ� */
	if (old > height) {	/* �ȑO����Ⴍ�Ȃ� */
		if (height >= 0) {
			/* �Ԃ̂�̂�����グ�� */
			for (h = old; h > height; h--) {
				ctl->sheets[h] = ctl->sheets[h - 1];
				ctl->sheets[h]->height = h;
			}
			ctl->sheets[height] = sht;
			sheet_refreshmap(ctl, sht->vx0, sht->vy0, sht->vx0 + sht->bxsize, sht->vy0 + sht->bysize, height + 1);
			sheet_refreshsub(ctl, sht->vx0, sht->vy0, sht->vx0 + sht->bxsize, sht->vy0 + sht->bysize, height + 1, old);
		} else {	/* ��\���� */
			if (ctl->top > old) {
				/* ��ɂȂ��Ă����̂�낷 */
				for (h = old; h < ctl->top; h++) {
					ctl->sheets[h] = ctl->sheets[h + 1];
					ctl->sheets[h]->height = h;
				}
			}
			ctl->top--; /* �\�����̉������������̂ŁA��ԏ�̍��������� */
			sheet_refreshmap(ctl, sht->vx0, sht->vy0, sht->vx0 + sht->bxsize, sht->vy0 + sht->bysize, 0);
			sheet_refreshsub(ctl, sht->vx0, sht->vy0, sht->vx0 + sht->bxsize, sht->vy0 + sht->bysize, 0, old - 1);
		}
	} else if (old < height) {	/* �ȑO�������Ȃ� */
		if (old >= 0) {
			/* �Ԃ̂�̂�������� */
			for (h = old; h < height; h++) {
				ctl->sheets[h] = ctl->sheets[h + 1];
				ctl->sheets[h]->height = h;
			}
			ctl->sheets[height] = sht;
		} else {	/* ��\����Ԃ���\����Ԃ� */
			/* ��ɂȂ��̂���グ�� */
			for (h = ctl->top; h >= height; h--) {
				ctl->sheets[h + 1] = ctl->sheets[h];
				ctl->sheets[h + 1]->height = h + 1;
			}
			ctl->sheets[height] = sht;
			ctl->top++; /* �\�����̉��������������̂ŁA��ԏ�̍����������� */
		}
		sheet_refreshmap(ctl, sht->vx0, sht->vy0, sht->vx0 + sht->bxsize, sht->vy0 + sht->bysize, height);
		sheet_refreshsub(ctl, sht->vx0, sht->vy0, sht->vx0 + sht->bxsize, sht->vy0 + sht->bysize, height, height);
	}
	return;
}

void sheet_refreshsub(struct SHTCTL *ctl,int vx0,int vy0,int vx1,int vy1,int h0,int h1)
{
	int h, bx, by, vx, vy, bx0, by0, bx1, by1;
	unsigned char *buf, *vram = ctl->vram,*map=ctl->map,sid;
	struct SHEET *sht;
	if(vx0<0) vx0=0;
	if(vy0<0) vy0=0;
	if(vx1>ctl->xsize)vx1=ctl->xsize;
	if(vy1>ctl->ysize)vy1=ctl->ysize; 
	
	for (h = h0; h <= h1; h++) {
		sht = ctl->sheets[h];
		buf = sht->buf;
		sid=sht-ctl->sheets0;
		/* vx0�`vy1��g���āAbx0�`by1��t�Z���� */
		bx0 = vx0 - sht->vx0;
		by0 = vy0 - sht->vy0;
		bx1 = vx1 - sht->vx0;
		by1 = vy1 - sht->vy0;
		if (bx0 < 0) { bx0 = 0; }
		if (by0 < 0) { by0 = 0; }
		if (bx1 > sht->bxsize) { bx1 = sht->bxsize; }
		if (by1 > sht->bysize) { by1 = sht->bysize; }
		for (by = by0; by < by1; by++) {
			vy = sht->vy0 + by;
			for (bx = bx0; bx < bx1; bx++) {
				vx = sht->vx0 + bx;
				if (map[vy*ctl->xsize+vx]==sid) {   //图层是最顶层
					vram[vy * ctl->xsize + vx] = buf[by * sht->bxsize + bx];
				}
			}
		}
	}
	return;
}

void sheet_refreshmap(struct SHTCTL *ctl, int vx0, int vy0, int vx1, int vy1, int h0)
{
	int h, bx, by, vx, vy, bx0, by0, bx1, by1;
	unsigned char *buf, sid, *map = ctl->map;
	struct SHEET *sht;
	if (vx0 < 0) { vx0 = 0; }
	if (vy0 < 0) { vy0 = 0; }
	if (vx1 > ctl->xsize) { vx1 = ctl->xsize; }
	if (vy1 > ctl->ysize) { vy1 = ctl->ysize; }
	for (h = h0; h <= ctl->top; h++) {
		sht = ctl->sheets[h];
		sid = sht - ctl->sheets0; //利用减法获取sheet ID
		buf = sht->buf;
		bx0 = vx0 - sht->vx0;
		by0 = vy0 - sht->vy0;
		bx1 = vx1 - sht->vx0;
		by1 = vy1 - sht->vy0;
		if (bx0 < 0) { bx0 = 0; }
		if (by0 < 0) { by0 = 0; }
		if (bx1 > sht->bxsize) { bx1 = sht->bxsize; }
		if (by1 > sht->bysize) { by1 = sht->bysize; }
		for (by = by0; by < by1; by++) {
			vy = sht->vy0 + by;
			for (bx = bx0; bx < bx1; bx++) {
				vx = sht->vx0 + bx;
				if (buf[by * sht->bxsize + bx] != sht->col_inv) {
					map[vy * ctl->xsize + vx] = sid;
				}
			}
		}
	}
	return;
}


void sheet_refresh(struct SHTCTL *ctl, struct SHEET *sht, int bx0, int by0, int bx1, int by1)
{
	if (sht->height >= 0) { /* ����\�����Ȃ�A�V�����������̏��ɉ����ĉ�ʂ�`������ */
		sheet_refreshsub(ctl, sht->vx0 + bx0, sht->vy0 + by0, sht->vx0 + bx1, sht->vy0 + by1,sht->height,sht->height);
	}
	return;
}

void sheet_slide(struct SHTCTL *ctl, struct SHEET *sht, int vx0, int vy0)
{
    int old_vx0=sht->vx0,old_vy0=sht->vy0;
	sht->vx0 = vx0;
	sht->vy0 = vy0;
	if (sht->height >= 0) { /* ����\�����Ȃ� */
		sheet_refreshmap(ctl, old_vx0, old_vy0, old_vx0 + sht->bxsize, old_vy0 + sht->bysize, 0);  //old 需要从底层开始刷新
		sheet_refreshmap(ctl, vx0, vy0, vx0 + sht->bxsize, vy0 + sht->bysize, sht->height);		//new 只需刷新顶层就ok
		sheet_refreshsub(ctl,old_vx0,old_vy0,old_vx0+sht->bxsize,old_vy0+sht->bysize,0,sht->height-1); /* �V�����������̏��ɉ����ĉ�ʂ�`������ */
        sheet_refreshsub(ctl,vx0,vy0,vx0+sht->bxsize,vy0+sht->bysize,sht->height,sht->height);
	}
	return;
}

void sheet_free(struct SHTCTL *ctl, struct SHEET *sht)
{
	if (sht->height >= 0) {
		sheet_updown(ctl, sht, -1); /* �\�����Ȃ�܂���\���ɂ��� */
	}
	sht->flags = 0; /* ���g�p�}�[�N */
	return;
}
