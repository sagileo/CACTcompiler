#include "./genAssemble.h"

list_head varlist;

list_head float_list;

int fnumber = 0;

std::string lookup(std::string id, int off = -1){
	struct address *entry, *q;
	list_for_each_entry_safe(entry, q, &varlist, list){
		if(id == entry->id && off == entry->off)
			return entry->addr;
	}
	return "notfound";
}

int global(std::string id){
	struct address *entry, *q;
	list_for_each_entry_safe(entry, q, &varlist, list){
		if(id == entry->id)
			return entry->type;
	}
	return 0;
}

std::string alloc(std::string id, int off, int width, int * seq){
	// struct address *tail = (struct address *)malloc(sizeof(struct address));
	address *tail = new address;
	init_list_head(&tail->list);
	tail->id = id;
	tail->off = off;
	tail->type = 2;
	std::string addr;
	if(*seq % width != 0) *seq = *seq + 4;
	std::string s = std::to_string(-1024 + *seq);
	*seq += width;
	addr = s + "(s0)";

	tail->addr = addr;
	tail->width = width;
	list_add_tail(&tail->list, &varlist);
	return addr;
}

void genDecl(FILE * fp, threeAdressCode code){
	init_list_head(&varlist);
	init_list_head(&float_list);
	fprintf(fp, "\t.text\n");
	int size = 0;
	for(int i = 0; i < code.cur_line; i++){
		struct address * entry;
		unsigned long data;
		if(code.lines[i].op == TACOP_NONE){
			size = *(int *)code.lines[i].arg1.data;
			continue;
		}
		if(code.lines[i].op == TACOP_LABEL)continue;
		if(code.lines[i].op == TACOP_ASSIGN){
			// entry = (struct address *)malloc(sizeof(struct address));
			entry = new address;
			init_list_head(&entry->list);
			entry->id = code.lines[i].result.temp;
			entry->off = -1;
			entry->type = 3;
			entry->width = code.lines[i].result.type == BTYPE_DOUBLE ? 8 : 4;
			entry->addr = code.lines[i].result.temp;
			data = *((unsigned long*)code.lines[i].arg1.data);
		}
		else if(code.lines[i].op == TACOP_WOFF){
			// entry = (struct address *)malloc(sizeof(struct address));
			data = *((unsigned long*)code.lines[i].arg2.data);
			if(lookup(code.lines[i].result.temp, 0) != "notfound"){
				fprintf(fp, "\t.word\t%u\n", data & 0xffffffff);
				if(code.lines[i].result.type == BTYPE_DOUBLE) fprintf(fp, "\t.word\t%u\n", data >> 32);
				continue;
			}
			entry = new address;
			init_list_head(&entry->list);
			entry->id = code.lines[i].result.temp;
			entry->off = *(int *)code.lines[i].arg1.data;
			entry->type = 3;
			entry->width = code.lines[i].result.type == BTYPE_DOUBLE ? 8 : 4;
			entry->addr = code.lines[i].result.temp + "_" + std::to_string(*(int *)code.lines[i].arg1.data);
		}
		fprintf(fp, "\t.globl\t%s\n", entry->addr.c_str());
		if(code.lines[i].op == TACOP_WOFF){
			fprintf(fp, "\t.data\n");
		}
		else if(code.lines[i].op == TACOP_ASSIGN){
			fprintf(fp, "\t.section\t.sdata,\"aw\"\n");
		}
		if(entry->type == BTYPE_DOUBLE) fprintf(fp, "\t.align\t3\n");
		else fprintf(fp, "\t.align\t2\n");
		fprintf(fp, "\t.type\t%s,@object\n", entry->addr.c_str());
		if(code.lines[i].op == TACOP_ASSIGN){
			fprintf(fp, "\t.size\t%s,%d\n", entry->addr.c_str(), entry->width);
		}
		else{
			fprintf(fp, "\t.size\t%s,%d\n", entry->addr.c_str(), size);
		}
		fprintf(fp, "%s:\n", entry->addr.c_str());
		fprintf(fp, "\t.word\t%u\n", data & 0xffffffff);
		if(entry->width == 8) fprintf(fp, "\t.word\t%u\n", data >> 32);
		list_add_tail(&entry->list, &varlist);
	}
	fprintf(fp, "\t.text\n");
}

void genFunc(FILE * fp, threeAdressCode code, struct paramList * param){
	int seq = 0;
	int paramSeq = 0;
	int fparamSeq = 0;

	//fprintf(fp, "\t.text\n");
	fprintf(fp, "\t.align\t1\n");
	int len = code.lines[0].arg1.temp.length();
	if(code.lines[0].arg1.temp != "main"){
		fprintf(fp, "\t.globl\t%s\n", code.lines[0].arg1.temp.c_str());
		fprintf(fp, "\t.type\t%s, @function\n", code.lines[0].arg1.temp.c_str());
		fprintf(fp, "%s:\n", code.lines[0].arg1.temp.c_str());
	}
	else{
		fprintf(fp, "\t.globl\tmain\n");
		fprintf(fp, "\t.type\tmain, @function\n");
		fprintf(fp, "main:\n");
	}
	fprintf(fp, "\taddi\tsp,sp,-1024\n");
	fprintf(fp, "\tsd\tra,1016(sp)\n");
	fprintf(fp, "\tsd\ts0,1008(sp)\n");
	fprintf(fp, "\taddi\ts0,sp,1024\n");

	if(param != nullptr){
		for(int j = 0; j < param->numParams; j++){
			int width0 = (param->p[j].btype == BTYPE_DOUBLE) ? 8 : 4;
			std::string re = alloc(param->p[j].name, -1, width0, &seq);
			if(param->p[j].btype == BTYPE_DOUBLE){
				fprintf(fp, "\tfsd\tfa%d,%s\n", fparamSeq, re.c_str());
				fparamSeq++;
			}
			else if(param->p[j].btype == BTYPE_FLOAT){
				fprintf(fp, "\tfsw\tfa%d,%s\n", fparamSeq, re.c_str());
				fparamSeq++;
			}
			else{
				fprintf(fp, "\tsw\ta%d,%s\n", paramSeq, re.c_str());
				paramSeq++;
			}
		}
		fparamSeq = 0;
		paramSeq = 0;
	}
	
	std::string arg1 = "notfound";
	std::string arg2 = "notfound";
	std::string result = "notfound";
	for(int i = 1; i < code.cur_line; i++){
		if(code.lines[i].arg1.imm == 0){
			arg1 = lookup(code.lines[i].arg1.temp);
		}
		if(code.lines[i].arg2.imm == 0){
			arg2 = lookup(code.lines[i].arg2.temp);
		}
		if(code.lines[i].result.imm == 0){
			result = lookup(code.lines[i].result.temp);
		}
		switch (code.lines[i].op)
		{
			case TACOP_NONE:
				fprintf(fp, "\tnop\n");
				break;
		
			case TACOP_ADD:
				if(result == "notfound"){
					int width = code.lines[i].result.type == BTYPE_DOUBLE ? 8 : 4;
					result = alloc(code.lines[i].result.temp, -1, width, &seq);
					//seq += 1;
				}

				if(code.lines[i].result.type == BTYPE_FLOAT){
					fprintf(fp, "\tflw\tfa4,%s\n", arg1.c_str());
					fprintf(fp, "\tflw\tfa5,%s\n", arg2.c_str());
					fprintf(fp, "\tfadd.s\tfa5,fa4,fa5\n");
					fprintf(fp, "\tfsw\tfa5,%s\n", result.c_str());
				}
				else if(code.lines[i].result.type == BTYPE_DOUBLE){
					fprintf(fp, "\tfld\tfa4,%s\n", arg1.c_str());
					fprintf(fp, "\tfld\tfa5,%s\n", arg2.c_str());
					fprintf(fp, "\tfadd.d\tfa5,fa4,fa5\n");
					fprintf(fp, "\tfsd\tfa5,%s\n", result.c_str());
				}
				else{
					fprintf(fp, "\tlw\ta4,%s\n", arg1.c_str());
					fprintf(fp, "\tlw\ta5,%s\n", arg2.c_str());
					fprintf(fp, "\taddw\ta5,a4,a5\n");
					fprintf(fp, "\tsw\ta5,%s\n", result.c_str());
				}
				break;
		
			case TACOP_SUB:
				if(result == "notfound"){
					int width = code.lines[i].result.type == BTYPE_DOUBLE ? 8 : 4;
					result = alloc(code.lines[i].result.temp, -1, width, &seq);
					//seq += 1;
				}

				if(code.lines[i].result.type == BTYPE_FLOAT){
					fprintf(fp, "\tflw\tfa4,%s\n", arg1.c_str());
					fprintf(fp, "\tflw\tfa5,%s\n", arg2.c_str());
					fprintf(fp, "\tfsub.s\tfa5,fa4,fa5\n");
					fprintf(fp, "\tfsw\tfa5,%s\n", result.c_str());
				}
				else if(code.lines[i].result.type == BTYPE_DOUBLE){
					fprintf(fp, "\tfld\tfa4,%s\n", arg1.c_str());
					fprintf(fp, "\tfld\tfa5,%s\n", arg2.c_str());
					fprintf(fp, "\tfsub.d\tfa5,fa4,fa5\n");
					fprintf(fp, "\tfsd\tfa5,%s\n", result.c_str());
				}
				else{
					fprintf(fp, "\tlw\ta4,%s\n", arg1.c_str());
					fprintf(fp, "\tlw\ta5,%s\n", arg2.c_str());
					fprintf(fp, "\tsubw\ta5,a4,a5\n");
					fprintf(fp, "\tsw\ta5,%s\n", result.c_str());
				}
				break;

			case TACOP_MUL:
				if(result == "notfound"){
					int width = code.lines[i].result.type == BTYPE_DOUBLE ? 8 : 4;
					result = alloc(code.lines[i].result.temp, -1, width, &seq);
					//seq += 1;
				}

				if(code.lines[i].result.type == BTYPE_FLOAT){
					fprintf(fp, "\tflw\tfa4,%s\n", arg1.c_str());
					fprintf(fp, "\tflw\tfa5,%s\n", arg2.c_str());
					fprintf(fp, "\tfmul.s\tfa5,fa4,fa5\n");
					fprintf(fp, "\tfsw\tfa5,%s\n", result.c_str());
				}
				else if(code.lines[i].result.type == BTYPE_DOUBLE){
					fprintf(fp, "\tfld\tfa4,%s\n", arg1.c_str());
					fprintf(fp, "\tfld\tfa5,%s\n", arg2.c_str());
					fprintf(fp, "\tfmul.d\tfa5,fa4,fa5\n");
					fprintf(fp, "\tfsd\tfa5,%s\n", result.c_str());
				}
				else{
					fprintf(fp, "\tlw\ta4,%s\n", arg1.c_str());
					fprintf(fp, "\tlw\ta5,%s\n", arg2.c_str());
					fprintf(fp, "\tmulw\ta5,a4,a5\n");
					fprintf(fp, "\tsw\ta5,%s\n", result.c_str());
				}
				break;

			case TACOP_DIV:
				if(result == "notfound"){
					int width = code.lines[i].result.type == BTYPE_DOUBLE ? 8 : 4;
					result = alloc(code.lines[i].result.temp, -1, width, &seq);
					//seq += 1;
				}

				if(code.lines[i].result.type == BTYPE_FLOAT){
					fprintf(fp, "\tflw\tfa4,%s\n", arg1.c_str());
					fprintf(fp, "\tflw\tfa5,%s\n", arg2.c_str());
					fprintf(fp, "\tfdiv.s\tfa5,fa4,fa5\n");
					fprintf(fp, "\tfsw\tfa5,%s\n", result.c_str());
				}
				else if(code.lines[i].result.type == BTYPE_DOUBLE){
					fprintf(fp, "\tfld\tfa4,%s\n", arg1.c_str());
					fprintf(fp, "\tfld\tfa5,%s\n", arg2.c_str());
					fprintf(fp, "\tfdiv.d\tfa5,fa4,fa5\n");
					fprintf(fp, "\tfsd\tfa5,%s\n", result.c_str());
				}
				else{
					fprintf(fp, "\tlw\ta4,%s\n", arg1.c_str());
					fprintf(fp, "\tlw\ta5,%s\n", arg2.c_str());
					fprintf(fp, "\tdivw\ta5,a4,a5\n");
					fprintf(fp, "\tsw\ta5,%s\n", result.c_str());
				}
				break;

			case TACOP_MOD:
				//if(line->arg1.c_str())
				if(result == "notfound"){
					int width = code.lines[i].result.type == BTYPE_DOUBLE ? 8 : 4;
					result = alloc(code.lines[i].result.temp, -1, width, &seq);
					//seq += 1;
				}
				fprintf(fp, "\tlw\ta4,%s\n", arg1.c_str());
				fprintf(fp, "\tlw\ta5,%s\n", arg2.c_str());
				fprintf(fp, "\tremw\ta5,a4,a5\n");
				fprintf(fp, "\tsw\ta5,%s\n", result.c_str());
				break;

			case TACOP_MINUS:
				if(result == "notfound"){
					int width = code.lines[i].result.type == BTYPE_DOUBLE ? 8 : 4;
					result = alloc(code.lines[i].result.temp, -1, width, &seq);
					//seq += 1;
				}
				if(code.lines[i].result.type == BTYPE_FLOAT){
					fprintf(fp, "\tflw\tfa5,%s\n", arg1.c_str());
					//fprintf(fp, "flw\tfa5,%s\n", arg2.c_str());
					fprintf(fp, "\tfneg.s\tfa5,fa5\n");
					fprintf(fp, "\tfsw\tfa5,%s\n", result.c_str());
				}
				else if(code.lines[i].result.type == BTYPE_DOUBLE){
					fprintf(fp, "\tfld\tfa5,%s\n", arg1.c_str());
					//fprintf(fp, "fld\tfa5,%s\n", arg2.c_str());
					fprintf(fp, "\tfneg.d\tfa5,fa5\n");
					fprintf(fp, "\tfsd\tfa5,%s\n", result.c_str());
				}
				else{
					fprintf(fp, "\tlw\ta5,%s\n", arg1.c_str());
					//fprintf(fp, "lw\ta5,%s\n", arg2.c_str());
					fprintf(fp, "\tnegw\ta5,a5\n");
					fprintf(fp, "\tsw\ta5,%s\n", result.c_str());
				}
				break;

			case TACOP_ROFF:	//result = arg1[arg2]
				arg1 = lookup(code.lines[i].arg1.temp, 0);
				int off;
				off = 0;
				sscanf(arg1.c_str(), "%d(s0)", &off);
				if(result == "notfound"){
					int width = code.lines[i].result.type == BTYPE_DOUBLE ? 8 : 4;
					result = alloc(code.lines[i].result.temp, -1, width, &seq);
					//seq += 1;
				}
				if(off == 0){
					if(code.lines[i].result.type == BTYPE_FLOAT){
							fprintf(fp, "\tlui\ta5,%%hi(%s)\n", arg1.c_str());
							fprintf(fp, "\taddi\ta5,a5,%%lo(%s)\n", arg1.c_str());
							fprintf(fp, "\tlw\ta4,%s\n", arg2.c_str());
							fprintf(fp, "\tadd\ta5,a4,a5\n");
							fprintf(fp, "\tflw\tfa4,0(a5)\n");
							fprintf(fp, "\tfsw\tfa4,%s\n", result.c_str());
					}
					else if(code.lines[i].result.type == BTYPE_DOUBLE){
							fprintf(fp, "\tlui\ta5,%%hi(%s)\n", arg1.c_str());
							fprintf(fp, "\taddi\ta5,a5,%%lo(%s)\n", arg1.c_str());
							fprintf(fp, "\tlw\ta4,%s\n", arg2.c_str());
							fprintf(fp, "\tadd\ta5,a4,a5\n");
							fprintf(fp, "\tfld\tfa4,0(a5)\n");
							fprintf(fp, "\tfsd\tfa4,%s\n", result.c_str());
					}
					else{
							fprintf(fp, "\tlui\ta5,%%hi(%s)\n", arg1.c_str());
							fprintf(fp, "\taddi\ta5,a5,%%lo(%s)\n", arg1.c_str());
							fprintf(fp, "\tlw\ta4,%s\n", arg2.c_str());
							fprintf(fp, "\tadd\ta5,a4,a5\n");
							fprintf(fp, "\tlw\ta4,0(a5)\n");
							fprintf(fp, "\tsw\ta4,%s\n", result.c_str());
					}
				}
				else{
					if(code.lines[i].result.type == BTYPE_FLOAT){
							fprintf(fp, "\tlw\ta5,%s\n", arg2.c_str());
							fprintf(fp, "\tadd\ta5,s0,a5\n");
							fprintf(fp, "\tflw\tfa4,%d(a5)\n", off);
							fprintf(fp, "\tfsw\tfa4,%s\n", result.c_str());
					}
					else if(code.lines[i].result.type == BTYPE_DOUBLE){
							fprintf(fp, "\tlw\ta5,%s\n", arg2.c_str());
							fprintf(fp, "\tadd\ta5,s0,a5\n");
							fprintf(fp, "\tfld\tfa4,%d(a5)\n", off);
							fprintf(fp, "\tfsd\tfa4,%s\n", result.c_str());
					}
					else{
							fprintf(fp, "\tlw\ta5,%s\n", arg2.c_str());
							fprintf(fp, "\tadd\ta5,s0,a5\n");
							fprintf(fp, "\tlw\ta4,%d(a5)\n", off);
							fprintf(fp, "\tsw\ta4,%s\n", result.c_str());
					}
				}
				break;

			case TACOP_WOFF:	//result[arg1] = arg2
				if(code.lines[i].arg1.imm == 1){
					int width = code.lines[i].result.type == BTYPE_DOUBLE ? 8 : 4;
					alloc(code.lines[i].result.temp, *(int *)code.lines[i].arg1.data, width, &seq);
					if(code.lines[i].result.type == BTYPE_DOUBLE){
						// struct floatc * f = (struct floatc *)malloc(sizeof(struct floatc));
						floatc *f = new floatc;
						init_list_head(&f->list);
						f->label = ".LC" + std::to_string(fnumber);
						f->data = malloc(8);
						memcpy(f->data, code.lines[i].arg1.data, 8);
						f->type = BTYPE_DOUBLE;
						list_add_tail(&f->list, &float_list);
						fnumber++;
						fprintf(fp, "\tlui\ta5,%%hi(%s)\n", f->label.c_str());
						fprintf(fp, "\tfld\tfa5,%%lo(%s)(a5)\n", f->label.c_str());
						fprintf(fp, "\tfsd\tfa5,%d(s0)\n", seq - 1024 - width);
					}
					else if(code.lines[i].result.type == BTYPE_FLOAT){
						// struct floatc * f = (struct floatc *)malloc(sizeof(struct floatc));
						floatc * f = new floatc;
						init_list_head(&f->list);
						f->label = ".LC" + std::to_string(fnumber);
						f->data = malloc(4);
						memcpy(f->data, code.lines[i].arg1.data, 8);
						f->type = BTYPE_FLOAT;
						list_add_tail(&f->list, &float_list);
						fnumber++;
						fprintf(fp, "\tlui\ta5,%%hi(%s)\n", f->label.c_str());
						fprintf(fp, "\tflw\tfa5,%%lo(%s)(a5)\n", f->label.c_str());
						fprintf(fp, "\tfsw\tfa5,%d(s0)\n", seq - 1024 - width);
					}
					else{
						fprintf(fp, "\tli\ta5,%d\n", *(int *)code.lines[i].arg2.data);
						fprintf(fp, "\tsw\ta5,%d(s0)\n", seq - 1024 - width);
					}
				}
				else{
					result = lookup(code.lines[i].result.temp, 0);
					int off = 0;
					sscanf(result.c_str(), "%d(s0)", &off);
					if(off == 0){
						if(code.lines[i].result.type == BTYPE_DOUBLE){
							fprintf(fp, "\tlui\ta5,%%hi(%s)\n", result.c_str());
							fprintf(fp, "\taddi\ta5,a5,%%lo(%s)\n", result.c_str());
							fprintf(fp, "\tlw\ta4,%s\n", arg1.c_str());
							fprintf(fp, "\tadd\ta5,a4,a5\n");
							fprintf(fp, "\tfld\tfa4,%s\n", arg2.c_str());
							fprintf(fp, "\tfsd\tfa4,0(a5)\n");
						}
						else if(code.lines[i].result.type == BTYPE_FLOAT){
							fprintf(fp, "\tlui\ta5,%%hi(%s)\n", result.c_str());
							fprintf(fp, "\taddi\ta5,a5,%%lo(%s)\n", result.c_str());
							fprintf(fp, "\tlw\ta4,%s\n", arg1.c_str());
							fprintf(fp, "\tadd\ta5,a4,a5\n");
							fprintf(fp, "\tflw\tfa4,%s\n", arg2.c_str());
							fprintf(fp, "\tfsw\tfa4,0(a5)\n");
						}
						else{
							fprintf(fp, "\tlui\ta5,%%hi(%s)\n", result.c_str());
							fprintf(fp, "\taddi\ta5,a5,%%lo(%s)\n", result.c_str());
							fprintf(fp, "\tlw\ta4,%s\n", arg1.c_str());
							fprintf(fp, "\tadd\ta5,a4,a5\n");
							fprintf(fp, "\tlw\ta4,%s\n", arg2.c_str());
							fprintf(fp, "\tsw\ta4,0(a5)\n");
						}
					}
					else{
						if(code.lines[i].result.type == BTYPE_DOUBLE){
							fprintf(fp, "\tlw\ta5,%s\n", arg1.c_str());
							fprintf(fp, "\tadd\ta5,s0,a5\n");
							fprintf(fp, "\tfld\tfa4,%s\n", arg2.c_str());
							fprintf(fp, "\tfsd\tfa4,%d(a5)\n", off);
						}
						else if(code.lines[i].result.type == BTYPE_FLOAT){
							fprintf(fp, "\tlw\ta5,%s\n", arg1.c_str());
							fprintf(fp, "\tadd\ta5,s0,a5\n");
							fprintf(fp, "\tflw\tfa4,%s\n", arg2.c_str());
							fprintf(fp, "\tfsw\tfa4,%d(a5)\n", off);
						}
						else{
							fprintf(fp, "\tlw\ta5,%s\n", arg1.c_str());
							fprintf(fp, "\tadd\ta5,s0,a5\n");
							fprintf(fp, "\tlw\ta4,%s\n", arg2.c_str());
							fprintf(fp, "\tsw\ta4,%d(a5)\n", off);
						}
					}
				}
				/* code */
				break;

			case TACOP_ASSIGN:
				if(result == "notfound"){
					int width = code.lines[i].result.type == BTYPE_DOUBLE ? 8 : 4;
					result = alloc(code.lines[i].result.temp, -1, width, &seq);
					//seq += 1;
				}
				if(code.lines[i].arg1.imm == 1){
					if(code.lines[i].arg1.type == BTYPE_FLOAT){
						// struct floatc * f = (struct floatc *)malloc(sizeof(struct floatc));
						floatc *f = new floatc;
						init_list_head(&f->list);
						f->label = ".LC" + std::to_string(fnumber);
						f->data = malloc(4);
						memcpy(f->data, code.lines[i].arg1.data, 4);
						f->type = BTYPE_FLOAT;
						list_add_tail(&f->list, &float_list);
						fnumber++;
						fprintf(fp, "\tlui\ta5,%%hi(%s)\n", f->label.c_str());
						fprintf(fp, "\tflw\tfa5,%%lo(%s)(a5)\n", f->label.c_str());
						fprintf(fp, "\tfsw\tfa5,%s\n", result.c_str());
					}
					else if(code.lines[i].arg1.type == BTYPE_DOUBLE){
						// struct floatc * f = (struct floatc *)malloc(sizeof(struct floatc));
						floatc *f = new floatc;
						init_list_head(&f->list);
						f->label = ".LC" + std::to_string(fnumber);
						f->data = malloc(8);
						memcpy(f->data, code.lines[i].arg1.data, 8);
						f->type = BTYPE_DOUBLE;
						list_add_tail(&f->list, &float_list);
						fnumber++;
						fprintf(fp, "\tlui\ta5,%%hi(%s)\n", f->label.c_str());
						fprintf(fp, "\tfld\tfa5,%%lo(%s)(a5)\n", f->label.c_str());
						fprintf(fp, "\tfsd\tfa5,%s\n", result.c_str());
					}
					else{
						fprintf(fp, "\tli\ta5,%d\n", *(int *)code.lines[i].arg1.data);
						fprintf(fp, "\tsw\ta5,%s\n", result.c_str());
					}
					break;
				}
				if(global(code.lines[i].arg1.temp) == 3 && global(code.lines[i].result.temp) == 3){
					if(code.lines[i].arg1.type == BTYPE_FLOAT){
						fprintf(fp, "\tlui\ta5,%%hi(%s)\n", arg1.c_str());
						fprintf(fp, "\tflw\tfa5,%%lo(%s)(a5)\n", arg1.c_str());
						fprintf(fp, "\tlui\ta5,%%hi(%s)\n", result.c_str());
						fprintf(fp, "\tfsw\tfa5,%%lo(%s)(a5)\n", result.c_str());
					}
					else if(code.lines[i].arg1.type == BTYPE_DOUBLE){
						fprintf(fp, "\tlui\ta5,%%hi(%s)\n", arg1.c_str());
						fprintf(fp, "\tfld\tfa5,%%lo(%s)(a5)\n", arg1.c_str());
						fprintf(fp, "\tlui\ta5,%%hi(%s)\n", result.c_str());
						fprintf(fp, "\tfsd\tfa5,%%lo(%s)(a5)\n", result.c_str());
					}
					else{
						fprintf(fp, "\tlui\ta5,%%hi(%s)\n", arg1.c_str());
						fprintf(fp, "\tlw\ta4,%%lo(%s)(a5)\n", arg1.c_str());
						fprintf(fp, "\tlui\ta5,%%hi(%s)\n", result.c_str());
						fprintf(fp, "\tsw\ta4,%%lo(%s)(a5)\n", result.c_str());
					}
				}
				else if(global(code.lines[i].arg1.temp) != 3 && global(code.lines[i].result.temp) == 3){
					if(code.lines[i].arg1.type == BTYPE_FLOAT){
						fprintf(fp, "\tlui\ta5,%%hi(%s)\n", result.c_str());
						fprintf(fp, "\tflw\tfa5,%s\n", arg1.c_str());
						fprintf(fp, "\tfsw\tfa5,%%lo(%s)(a5)\n", result.c_str());
					}
					else if(code.lines[i].arg1.type == BTYPE_DOUBLE){
						fprintf(fp, "\tlui\ta5,%%hi(%s)\n", result.c_str());
						fprintf(fp, "\tfld\tfa5,%s\n", arg1.c_str());
						fprintf(fp, "\tfsd\tfa5,%%lo(%s)(a5)\n", result.c_str());
					}
					else{
						fprintf(fp, "\tlui\ta5,%%hi(%s)\n", result.c_str());
						fprintf(fp, "\tlw\ta4,%s\n", arg1.c_str());
						fprintf(fp, "\tsw\ta4,%%lo(%s)(a5)\n", result.c_str());
					}
				}
				else if(global(code.lines[i].arg1.temp) == 3 && global(code.lines[i].result.temp) != 3){
					if(code.lines[i].arg1.type == BTYPE_FLOAT){
						fprintf(fp, "\tlui\ta5,%%hi(%s)\n", arg1.c_str());
						fprintf(fp, "\tflw\tfa5,%%lo(%s)(a5)\n", arg1.c_str());
						fprintf(fp, "\tfsw\tfa5,%s\n", result.c_str());
					}
					else if(code.lines[i].arg1.type == BTYPE_DOUBLE){
						fprintf(fp, "\tlui\ta5,%%hi(%s)\n", arg1.c_str());
						fprintf(fp, "\tfld\tfa5,%%lo(%s)(a5)\n", arg1.c_str());
						fprintf(fp, "\tfsd\tfa5,%s\n", result.c_str());
					}
					else{
						fprintf(fp, "\tlui\ta5,%%hi(%s)\n", arg1.c_str());
						fprintf(fp, "\tlw\ta4,%%lo(%s)(a5)\n", arg1.c_str());
						fprintf(fp, "\tsw\ta4,%s\n", result.c_str());
					}
				}
				else{
					if(code.lines[i].arg1.type == BTYPE_FLOAT){
						fprintf(fp, "\tflw\tfa5,%s\n", arg1.c_str());
						fprintf(fp, "\tfsw\tfa5,%s\n", result.c_str());
					}
					else if(code.lines[i].arg1.type == BTYPE_DOUBLE){
						fprintf(fp, "\tfld\tfa5,%s\n", arg1.c_str());
						fprintf(fp, "\tfsd\tfa5,%s\n", result.c_str());
					}
					else{
						fprintf(fp, "\tlw\ta5,%s\n", arg1.c_str());
						fprintf(fp, "\tsw\ta5,%s\n", result.c_str());
					}
				}
				/* code */
				break;

			case TACOP_PARAM:
				if(code.lines[i].arg1.type == BTYPE_FLOAT){
					fprintf(fp, "\tflw\tfa%d,%s\n", fparamSeq, arg1.c_str());
					fparamSeq += 1;
				}
				else if(code.lines[i].arg1.type == BTYPE_DOUBLE){
					fprintf(fp, "\tfld\tfa%d,%s\n", fparamSeq, arg1.c_str());
					fparamSeq += 1;
				}else{
					fprintf(fp, "\tlw\ta%d,%s\n", paramSeq, arg1.c_str());
					paramSeq += 1;
				}		
				break;

			case TACOP_CALL:
				paramSeq = 0;
				fparamSeq = 0;
				fprintf(fp, "\tcall\t%s\n", code.lines[i].arg1.temp.c_str());
				if(code.lines[i].result.imm == 0){
					if(result == "notfound"){
						int width = code.lines[i].result.type == BTYPE_DOUBLE ? 8 : 4;
						result = alloc(code.lines[i].result.temp, -1, width, &seq);
						//seq += 1;
					}
					if(code.lines[i].result.type == BTYPE_DOUBLE){
						fprintf(fp, "\tfsd\tfa0,%s\n", result.c_str());
					}
					else if(code.lines[i].result.type == BTYPE_FLOAT){
						fprintf(fp, "\tfsw\tfa0,%s\n", result.c_str());
					}
					else{
						fprintf(fp, "\tsw\ta0,%s\n", result.c_str());
					}
				}
				/* code */
				break;

			case TACOP_RETURN:
				if(code.lines[i].arg1.imm == 0){
					if(code.lines[i].arg1.type == BTYPE_DOUBLE){
						fprintf(fp, "\tfld\tfa0,%s\n", arg1.c_str());
					}
					else if(code.lines[i].arg1.type == BTYPE_FLOAT){
						fprintf(fp, "\tflw\tfa0,%s\n", arg1.c_str());
					}
					else{
						fprintf(fp, "\tlw\ta0,%s\n", arg1.c_str());
					}
				}
				fprintf(fp, "\tld\tra,1016(sp)\n");
				fprintf(fp, "\tld\ts0,1008(sp)\n");
				fprintf(fp, "\taddi\tsp,sp,1024\n");
				fprintf(fp, "\tjr\tra\n");
				/* code */
				break;

			case TACOP_J_LT:
				if(code.lines[i].arg1.type == BTYPE_FLOAT){
					fprintf(fp, "\tflw\tfa4,%s\n", arg1.c_str());
					fprintf(fp, "\tflw\tfa5,%s\n", arg2.c_str());
					fprintf(fp, "\tflt.s\ta5,fa4,fa5\n");
					fprintf(fp, "\tbeq\ta5,zero,%s\n", code.lines[i].result.temp.c_str());
				}
				else if(code.lines[i].arg1.type == BTYPE_DOUBLE){
					fprintf(fp, "\tfld\tfa4,%s\n", arg1.c_str());
					fprintf(fp, "\tfld\tfa5,%s\n", arg2.c_str());
					fprintf(fp, "\tflt.d\ta5,fa4,fa5\n");
					fprintf(fp, "\tbeq\ta5,zero,%s\n", code.lines[i].result.temp.c_str());
				}
				else{
					fprintf(fp, "\tlw\ta4,%s\n", arg1.c_str());
					fprintf(fp, "\tlw\ta5,%s\n", arg2.c_str());
					fprintf(fp, "\tblt\ta4,a5,%s\n", code.lines[i].result.temp.c_str());
				}
				/* code */
				break;

			case TACOP_J_LE:
				if(code.lines[i].arg1.type == BTYPE_FLOAT){
					fprintf(fp, "\tflw\tfa4,%s\n", arg1.c_str());
					fprintf(fp, "\tflw\tfa5,%s\n", arg2.c_str());
					fprintf(fp, "\tfle.s\ta5,fa4,fa5\n");
					fprintf(fp, "\tbeq\ta5,zero,%s\n", code.lines[i].result.temp.c_str());
				}
				else if(code.lines[i].arg1.type == BTYPE_DOUBLE){
					fprintf(fp, "\tfld\tfa4,%s\n", arg1.c_str());
					fprintf(fp, "\tfld\tfa5,%s\n", arg2.c_str());
					fprintf(fp, "\tfle.d\ta5,fa4,fa5\n");
					fprintf(fp, "\tbeq\ta5,zero,%s\n", code.lines[i].result.temp.c_str());
				}
				else{
					fprintf(fp, "\tlw\ta4,%s\n", arg1.c_str());
					fprintf(fp, "\tlw\ta5,%s\n", arg2.c_str());
					fprintf(fp, "\tble\ta4,a5,%s\n", code.lines[i].result.temp.c_str());
				}
				/* code */
				break;

			case TACOP_J_GT:
				if(code.lines[i].arg1.type == BTYPE_FLOAT){
					fprintf(fp, "\tflw\tfa4,%s\n", arg1.c_str());
					fprintf(fp, "\tflw\tfa5,%s\n", arg2.c_str());
					fprintf(fp, "\tfgt.s\ta5,fa4,fa5\n");
					fprintf(fp, "\tbeq\ta5,zero,%s\n", code.lines[i].result.temp.c_str());
				}
				else if(code.lines[i].arg1.type == BTYPE_DOUBLE){
					fprintf(fp, "\tfld\tfa4,%s\n", arg1.c_str());
					fprintf(fp, "\tfld\tfa5,%s\n", arg2.c_str());
					fprintf(fp, "\tfgt.d\ta5,fa4,fa5\n");
					fprintf(fp, "\tbeq\ta5,zero,%s\n", code.lines[i].result.temp.c_str());
				}
				else{
					fprintf(fp, "\tlw\ta4,%s\n", arg1.c_str());
					fprintf(fp, "\tlw\ta5,%s\n", arg2.c_str());
					fprintf(fp, "\tbgt\ta4,a5,%s\n", code.lines[i].result.temp.c_str());
				}
				/* code */
				break;

			case TACOP_J_GE:
				if(code.lines[i].arg1.type == BTYPE_FLOAT){
					fprintf(fp, "\tflw\tfa4,%s\n", arg1.c_str());
					fprintf(fp, "\tflw\tfa5,%s\n", arg2.c_str());
					fprintf(fp, "\tfge.s\ta5,fa4,fa5\n");
					fprintf(fp, "\tbeq\ta5,zero,%s\n", code.lines[i].result.temp.c_str());
				}
				else if(code.lines[i].arg1.type == BTYPE_DOUBLE){
					fprintf(fp, "\tfld\tfa4,%s\n", arg1.c_str());
					fprintf(fp, "\tfld\tfa5,%s\n", arg2.c_str());
					fprintf(fp, "\tfge.d\ta5,fa4,fa5\n");
					fprintf(fp, "\tbeq\ta5,zero,%s\n", code.lines[i].result.temp.c_str());
				}
				else{
					fprintf(fp, "\tlw\ta4,%s\n", arg1.c_str());
					fprintf(fp, "\tlw\ta5,%s\n", arg2.c_str());
					fprintf(fp, "\tbge\ta4,a5,%s\n", code.lines[i].result.temp.c_str());
				}
				/* code */
				break;

			case TACOP_J_EQ:
				if(code.lines[i].arg1.type == BTYPE_FLOAT){
					fprintf(fp, "\tflw\tfa4,%s\n", arg1.c_str());
					fprintf(fp, "\tflw\tfa5,%s\n", arg2.c_str());
					fprintf(fp, "\tfeq.s\ta5,fa4,fa5\n");
					fprintf(fp, "\tbeq\ta5,zero,%s\n", code.lines[i].result.temp.c_str());
				}
				else if(code.lines[i].arg1.type == BTYPE_DOUBLE){
					fprintf(fp, "\tfld\tfa4,%s\n", arg1.c_str());
					fprintf(fp, "\tfld\tfa5,%s\n", arg2.c_str());
					fprintf(fp, "\tfeq.d\ta5,fa4,fa5\n");
					fprintf(fp, "\tbeq\ta5,zero,%s\n", code.lines[i].result.temp.c_str());
				}
				else{
					fprintf(fp, "\tlw\ta4,%s\n", arg1.c_str());
					fprintf(fp, "\tlw\ta5,%s\n", arg2.c_str());
					fprintf(fp, "\tbeq\ta4,a5,%s\n", code.lines[i].result.temp.c_str());
				}
				/* code */
				break;

			case TACOP_J_NE:
				if(code.lines[i].arg1.type == BTYPE_FLOAT){
					fprintf(fp, "\tflw\tfa4,%s\n", arg1.c_str());
					fprintf(fp, "\tflw\tfa5,%s\n", arg2.c_str());
					fprintf(fp, "\tfne.s\ta5,fa4,fa5\n");
					fprintf(fp, "\tbeq\ta5,zero,%s\n", code.lines[i].result.temp.c_str());
				}
				else if(code.lines[i].arg1.type == BTYPE_DOUBLE){
					fprintf(fp, "\tfld\tfa4,%s\n", arg1.c_str());
					fprintf(fp, "\tfld\tfa5,%s\n", arg2.c_str());
					fprintf(fp, "\tfne.d\ta5,fa4,fa5\n");
					fprintf(fp, "\tbeq\ta5,zero,%s\n", code.lines[i].result.temp.c_str());
				}
				else{
					fprintf(fp, "\tlw\ta4,%s\n", arg1.c_str());
					fprintf(fp, "\tlw\ta5,%s\n", arg2.c_str());
					fprintf(fp, "\tbne\ta4,a5,%s\n", code.lines[i].result.temp.c_str());
				}
				/* code */
				break;

			case TACOP_J:
				fprintf(fp, "\tj\t%s\n", code.lines[i].arg1.temp.c_str());
				/* code */
				break;

			case TACOP_LABEL:
				fprintf(fp, "%s:\n", code.lines[i].arg1.temp.c_str());
				/* code */
				break;

			default:	
				break;
		}
	}
	//for循环结束
	if(code.lines[0].arg1.temp != "main"){
		fprintf(fp, "\t.size\t%s, .-%s\n", code.lines[0].arg1.temp.c_str(), code.lines[0].arg1.temp.c_str());
	}
	else{
		fprintf(fp, "\t.size\tmain, .-main\n");
	}
	

}

void genFloat(FILE * fp){
	if(list_empty(&float_list))return;
	struct floatc * f;
	fprintf(fp, "\t.section\t.rodata\n");
	list_for_each_entry(f, &float_list, list){
		if(f->type == BTYPE_DOUBLE){
			fprintf(fp, "\t.align\t3\n");
			fprintf(fp, "%s:\n", f->label.c_str());
			unsigned long d = *(unsigned long *)f->data;
			fprintf(fp, "\t.word\t%d\n", d & 0xffffffff);
			fprintf(fp, "\t.word\t%d\n", d >> 32);
		}
		else{
			fprintf(fp, "\t.align\t2\n");
			fprintf(fp, "%s:\n", f->label.c_str());
			int d = *(int *)f->data;
			fprintf(fp, "\t.word\t%d\n", d);
		}
	}
}