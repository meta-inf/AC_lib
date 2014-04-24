/*
 * Yet another big integer class
 * Code by sumneng94
 * brute force
 * */
#include <cstdio>
#include <cmath>
#include <iostream>
#include <cstring>

using namespace std;

const int P=10000,L=5000,W=4;
char s[L*W];
struct Big
  {
    int len;int data[L];bool fu;
    void clear() 
      {
        memset(data,0,sizeof(data));
        len=0;fu=false;
      }
    int& operator [] (int k)
      {
        return data[k];
      }
    void operator = (int k)
      {
        clear();
        if (k<0) fu=true,k=-k;else fu=false;
        len=0;
        while (k) data[++len]=k%P,k/=P; 
        if (len==0) len=1;
      }
    bool operator < (Big b)
      {
        bool t=false;
        if (fu && !b.fu) return true;
        if (!fu && b.fu) return false;
        if (fu && b.fu) t=true;
        if (len<b.len) return true^t;
        if (len>b.len) return false^t;
        for (int i=len;i;--i)
          {
            if (data[i]<b[i]) return true^t;
            if (data[i]>b[i]) return false^t;
          }
        return false;
      }
    bool operator <= (Big b)
      {
        bool t=false;
        if (fu && !b.fu) return true;
        if (!fu && b.fu) return false;
        if (fu && b.fu) t=true;
        if (len<b.len) return true^t;
        if (len>b.len) return false^t;
        for (int i=len;i;--i)
          {
            if (data[i]<b[i]) return true^t;
            if (data[i]>b[i]) return false^t;
          }
        return true;
      }
    bool operator > (Big b)
      {
        bool t=false;
        if (fu && !b.fu) return false;
        if (!fu && b.fu) return true;
        if (fu && b.fu) t=true;
        if (len<b.len) return false^t;
        if (len>b.len) return true^t;
        for (int i=len;i;--i)
          {
            if (data[i]<b[i]) return false^t;
            if (data[i]>b[i]) return true^t;
          }
        return false;
      }
    bool operator >= (Big b)
      {
        bool t=false;
        if (fu && !b.fu) return false;
        if (!fu && b.fu) return true;
        if (fu && b.fu) t=true;
        if (len<b.len) return false^t;
        if (len>b.len) return true^t;
        for (int i=len;i;--i)
          {
            if (data[i]<b[i]) return false^t;
            if (data[i]>b[i]) return true^t;
          }
        return true;
      }
    bool operator == (Big b)
      {
        if (fu!=b.fu) return false;
        if (len<b.len) return false;
        if (len>b.len) return false;
        for (int i=len;i;--i)
          if (data[i]!=b[i]) return false;
        return true;
      }
    bool operator == (int k)
      {
        if (k<0)
          {
            if (!fu) return false;
            k=-k;
          } else if (fu) return false;
        if (k>=P)
          {
            Big b;b=k;
            return *this==b;
          }
            else return len==1 && data[1]==k;
      }
    bool operator != (Big b)
      {
        if (fu!=b.fu) return true;
        if (len<b.len) return true;
        if (len>b.len) return true;
        for (int i=len;i;--i)
          if (data[i]!=b[i]) return true;
        return false;
      }
    bool operator != (int k)
      {
        if (k<0)
          {
            if (!fu) return true;
            k=-k;
          } else if (fu) return true;
        if (k>=P)
          {
            Big b;b=k;
            return *this!=b;
          }
            else return !(len==1 && data[1]==k);
      }
    Big operator + (Big b)
      {
        Big a=*this,c;c.clear();
        if (a.fu && b.fu) 
          {
            a.fu=false;b.fu=false;c=a+b;
            if (c.len!=1 || c[1]!=0) c.fu=true;
            return c;
          }
        if (a.fu && !b.fu) 
          {a.fu=false;return b-a;}
        if (!a.fu && b.fu)
          {b.fu=false;return a-b;} 
        a.len=max(a.len,b.len);
        for (int i=1;i<=a.len;++i)
          {
            a[i+1]+=(a[i]+b[i])/P;
            a[i]=(a[i]+b[i])%P;
          }
        if (a[a.len+1]) ++a.len;
        while (a[a.len]==0 && a.len>1) --a.len;
        return a;
      }
    Big operator + (int k)
      {
        Big a=*this,b;b=k;
        return a+b;
      }
    Big operator - (Big b)
      {
        Big a=*this,c;c.clear();
        if (a.fu && !b.fu)
          {
            a.fu=false;b.fu=false;c=a+b;
            if (c.len!=1 || c[1]!=0) c.fu=true;
            return c;
          }
        if (a.fu && b.fu) 
          {
            a.fu=false;b.fu=false;return b-a;
          }
        if (!a.fu && b.fu)
          {
            b.fu=false; return a+b;
          }
        if (a<b) swap(a,b),a.fu=true;else a.fu=false;
        for (int i=1;i<=a.len;++i)
          {
            if (a[i]<b[i]) a[i]+=P,--a[i+1];
            a[i]-=b[i];
          }
        while (a[a.len]==0 && a.len>1) --a.len;
        if (a.len==1 && a[1]==0) a.fu=false;
        return a;
      }
    Big operator - (int k)
      {
        Big a=*this,b;b=k;
        return a-b;
      }
    Big operator * (Big b)
      {
        Big c;c.clear();
        c.len=len+b.len-1;
        for (int i=1;i<=len;++i)
          for (int j=1;j<=b.len;++j)
            {
              c[i+j-1]+=data[i]*b[j];
              c[i+j]+=c[i+j-1]/P;
              c[i+j-1]%=P;
            }
        if (c[c.len+1]) ++c.len;
        while (c[c.len]==0 && c.len>1) --c.len;
        c.fu=fu^b.fu;
        if (c.len==1 && c[1]==0) c.fu=false;
        return c;
      }
    Big operator * (int k)
      {
        Big a=*this;
        if (k<0) a.fu=!a.fu,k=-k;
        if (k>=P) 
          {
            Big b;b=k;
            return a*b;
          }
        for (int i=1;i<=a.len;++i) a[i]*=k;
        for (int i=1;i<=a.len;++i)
          a[i+1]+=a[i]/P,a[i]%=P;
        while (a[a.len+1]) 
          {
            ++a.len;
            a[a.len+1]=a[a.len]/P;
            a[a.len]%=P;
          }
        while (a[a.len]==0 && a.len>1) --a.len;
        if (a.len==1 && a[1]==0) a.fu=false;
        return a;
      }
    Big operator / (int k)
      {
        Big a=*this;int g=0;
        if (k<0) a.fu=!a.fu,k=-k;
        for (int i=a.len;i;--i)
          {
            a[i]+=g*P;
            g=a[i]%k;
            a[i]/=k;
          }
        while (a[a.len]==0 && a.len>1) --a.len;
        if (a.len==1 && a[1]==0) a.fu=false;
        return a;
      }
    Big operator % (int k)
      {
        Big b;b=k;
        return *this%b;
      }
    Big operator / (Big b)
      {
        Big c,d;c=0;d=0;c.fu=fu^b.fu;b.fu=false;
        for (int i=len;i;--i)
          {
            d=d*P+data[i];
            int ans=0,l=0,r=P-1;
            while (l<=r)
              {
                int mid=(l+r)>>1;
                if (b*mid<=d) ans=mid,l=mid+1;
                else r=mid-1;
              }
            c[i]=ans;
            d=d-b*c[i];
          }
        c.len=len;
        while (c[c.len]==0 && c.len>1) --c.len;
        return c;
      }
    Big operator % (Big b)
      {
        Big c,d;c=0;d=0;c.fu=fu^b.fu;b.fu=false;
        for (int i=len;i;--i)
          {
            d=d*P+data[i];
            int ans=0,l=0,r=P-1;
            while (l<=r)
              {
                int mid=(l+r)>>1;
                if (b*mid<=d) ans=mid,l=mid+1;
                else r=mid-1;
              }
            c[i]=ans;
            d=d-b*c[i];
          }
        c.len=len;
        while (c[c.len]==0 && c.len>1) --c.len;
        d=*this-b*c;
        return d;
      }
    Big operator ^ (Big b)
      {
        Big ans,i;ans=1;
        for (i=1;i<=b;i=i+1) ans=ans*(*this);
        return ans;
      }    
    void read()
      {
        scanf("%s",s);
        clear();
        len=1;
        int pow=1,t=1,l=strlen(s),stop=0;
        if (s[0]=='-') fu=true,stop=1;
        for (int i=l-1;i>=stop;--i)
          {
            if (t>W) t=pow=1,++len;
            data[len]+=pow*(s[i]-'0');
            ++t,pow*=10;
          }
      }
    void write()
      {
        if (fu) printf("%c",'-');
        printf("%d",data[len]);
        for (int i=len-1;i;--i)
          {
            if (data[i]<10) putchar('0');
            if (data[i]<100) putchar('0');
            if (data[i]<1000) putchar('0');
            printf("%d",data[i]);
          }
        printf("\n");
      }
  };
struct Big n;
int main()
  {
  }
