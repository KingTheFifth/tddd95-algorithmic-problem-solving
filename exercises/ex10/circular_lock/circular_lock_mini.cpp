#include <bits/stdc++.h>
using namespace std;int main(){int n,s,t,u,v,w,x,y,z,p;cin>>n;while(n-->0)cin>>s>>t>>w>>x>>u>>v>>y>>z,p=gcd(gcd(gcd(w,x),y),z),cout<<((s+v)%p==(t+u)%p?"Yes\n":"No\n");}
