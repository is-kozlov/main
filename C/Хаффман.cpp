#include <iostream>
#include <stdlib.h>
#include <vector>

using namespace std;

#include <stdio.h>


struct pqnode 
{
	int priority;
	void *value;
	pqnode() 
    {
		priority = -1;
		value = NULL;
	}
};

class priority_queue 
{
    public:
    	priority_queue(int MAX) 
        {
    		body = new pqnode[maxsize = MAX + 1];
    		numnodes = 0;
    	}
    
    	~priority_queue() 
        {
    		delete[] body;
    	}
    
    	void heapify(int index) 
        {
    		for (;;) 
            {
    			int left = index + index; int right = left + 1;
    			// ��� ������, [index], [left], [right]?
    			//���������� ������
    			int largest = index;
    			if (left <= numnodes &&
    				body[left].priority < body[index].priority)
    				largest = left;
    			if (right <= numnodes &&
    				body[right].priority < body[largest].priority)
    				largest = right;
    			if (largest == index) break;
    			swap(index, largest);
    			index = largest;
    		}
    	}
    
    	void swap(int n1, int n2) 
        {
    		pqnode t = body[n1];
    		body[n1] = body[n2];
    		body[n2] = t;
    	}
    
    	int insert(pqnode const &node) 
        {
    		if (numnodes > maxsize) 
            {
    			return -1; // ��� ���������.
    		}
    		body[++numnodes] = node;
    		for (int i = numnodes; i > 1 &&
    			body[i].priority > body[i / 2].priority;
    			i /= 2) 
            {
    			swap(i, i / 2);
    		}
    		return 0;
    	}
    
    	pqnode fetchMin() 
        {
    		return numnodes == 0 ? pqnode() : body[1];
    	}
    
    	int removeMin() 
        {
    		if (numnodes == 0) return -1;
    		body[1] = body[numnodes--];
    		heapify(1);
    		return 0;
    	}
    
    	void print() 
        {
    		printf("max=%d size=%d elems={ ", maxsize, numnodes);
    		for (int i = 0; i < numnodes; i++) {
    			printf("%d ", body[i].priority);
    		}
    		printf("}\n");
    	}
    
    	int size() 
        {
    		return numnodes;
    	}
    	
    private:
    	pqnode *body;
    	int numnodes;
    	int maxsize;
};

//------------------------------------------------------------------------------------



struct hnode  //������ ��������
{
    int freq;
    char sym;
    hnode *child[2];
    
    hnode()
    {
        freq = 0;
        sym = -1;
        child[0] = NULL, child[1] = NULL;
    }
};



void walk(hnode *n, string *codes, string s)  //������ �� ������ ��������, ���������� ���� ������� � ���� ����� � ������
{
    if(n->sym >= 0)  //������������ ������� - ���������� ������ � ������ ����� � �������
    {
        codes[n->sym] = s;
        return;
    }
    walk(n->child[0], codes, s+"0");
    walk(n->child[1], codes, s+"1");
}


int main()
{
    char c;
    int N = 256, k = 0;
    int freq[256] = {};  //������� ������������� ��������
    /*
    memset(freq, 0, sizeof freq);  //������� ���������
    */
    
    FILE *in = fopen("input.txt", "r");
    
    

    while((c = getc(in)) != EOF)
    {
        freq[c] ++;
    }


    
    
    priority_queue pq(256);
    for(int i = 0; i < N; i++)  //���������� ������ ������ � ������ �������� ��������� ������ � �������� ��� ������� � ������������ �������
    {
        if(freq[i] == 0) continue;  //�� ����������� - ����������
        hnode *h = new hnode();
        h->freq = freq[i];
        h->sym = i;  //������
        pqnode p;  //������� ������ ������������ �������
        p.priority = h->freq;
        p.value = (void*) h;
        pq.insert(p);
    }
    
    while(pq.size() > 1)  //�������� �� ������������ �������
    //������ ��� ��������� ��� ������ � ���������� ����������� � ���������� �� � ����, ������ ������� � �������
    //����������� ������ ������ ��������
    {
        pqnode right = pq.fetchMin();
        pq.removeMin();
        pqnode left = pq.fetchMin();
        pq.removeMin();
        pqnode summ;  //������� ������������ ������� - ������ ��������� �� ������� ������ �������� (��� �������������)
        summ.priority = left.priority + right.priority;
        hnode *n = new hnode();  //������ ��������, ������� ����� ������ � ���� ������ �������
        n->child[0] = (hnode *) left.value;  //����� � ������ ����������
        n->child[1] = (hnode *) right.value;
        summ.value =(void *) n;
        pq.insert(summ);
    }
    //hnode *r = (hnode *) pq.fetchMin(); 
    hnode *r = (hnode *) pq.fetchMin().value;  //��������� ���������� ������ � ������� - ������� ������ ��������
    
    string codes[256] = {};  //���� �������� ��� ������� �������
    walk(r, codes, "");
    
    /*
    for(int i = 0; i < N; i++)
    {
        cout << codes[i] << endl;
    }
    */
    
    //---------------------------------------------------------------------------
    
    FILE *out = fopen("output.txt", "w");
    
    for(int i = 0; i < 256; i++)  //�������� � ���� ����
    {
        if(codes[i].size() > 0) fprintf(out, "%d:%s\n", i, codes[i].c_str());
    }
    fprintf(out, "@\n");
    
    rewind(in);  //��������� � ������ �����
    
    for(int c = getc(in); c != EOF; c = getc(in))
    {
        fprintf(out, "%s", codes[c].c_str());
    }
    
    fclose(in);
    fclose(out);
    
    //----------------------------------------------------------------------------
    
    in = fopen("output.txt", "r");
    
    hnode *root = new hnode();
    int c1;
    char cc[256];
    
    
    
    while(fscanf(in, "%d:%s\n", &c1, cc))  //������ ���� � ������ �������� � ��������������� ������ ��������
    {
        hnode *curr = root;
        for(int i = 0; cc[i] != 0; i++)
        {
            if(curr->child[cc[i] - '0'] == NULL)
            {
                curr->child[cc[i] - '0'] = new hnode();
            }
            curr = curr->child[cc[i] - '0'];
        }
        curr->sym = c1;
    }
    
    
    while((c = getc(in)) == '\n'); //���������� �������������� ������
    ungetc(c, in);
    
    c = getc(in); 
    c = getc(in);  //���������� @\n
    
    
    hnode *curr = root;
    for(c = getc(in); c != EOF; c = getc(in))  //������ ������������� ������ � ������� �����������
    {
        //printf("%c ", c);
        curr = curr -> child[c - '0'];
        if(curr->sym >= 0)
        {
            printf("%c", curr->sym);
            curr = root;
        }
    }
    
    
    cin >> c;
    
}
