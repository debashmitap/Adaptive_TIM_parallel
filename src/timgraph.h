class TimGraph: public InfGraph
{
    public:
        int n_prime;

        TimGraph(string folder, string graph_file):InfGraph(folder, graph_file ){
            visitedNodes = vector<bool>(n+1, false);
            readActivated(folder+"visited.txt");   
        }
        void readActivated(string path){
            fstream fin;
            fin.open((path).c_str(), ios::in);
            int readCnt=0;
            string tp;
            while (getline(fin, tp)) {
                stringstream s(tp);
                readCnt ++;
                int a;
                s >> a;

                visitedNodes[a] = true;
            }
            fin.close();
        }
        double MgT(int u){
            //static int i=0;
            //i++;
            //TRACE_LINE("mgt", i);
            ASSERT(u>=0);
            ASSERT(u<n);
            return (double)BuildHypergraphNode(u, 0, false);
        }
        double algo2(){
            double lb=1/2.0;
            double c=0;
            if(n_prime == 1)
            {
                return 1;
            }
            while(true){
                int loop= (6 * log(n_prime)  +  6 * log(log(n_prime)/ log(2)) )* 1/lb  ;
                c=0;
                IF_TRACE(int64 now=rdtsc());
                double sumMgTu=0;
                for(int i=0; i<loop; i++){
                    int u=rand()%n;
                    double MgTu=MgT(u);
                    double pu=MgTu/m;
                    sumMgTu+=MgTu;
                    c+=1-pow((1-pu), k);
                }
                c/=loop;
                if(c>lb) break;
                lb /= 2;
            }
            return c * n_prime;
        }
        double KptEstimation()
        {
            Timer t(1, "step1");
            double ept=algo2();
            ept/=2;
            return ept;
        }
        void RefindKPT(double epsilon, double ept){
            Timer t(2, "step2" );
            ASSERT(ept > 0);
            cout << "EPT: " << ept << endl;
            int64 R = (2 + epsilon) * ( n_prime * log(n_prime) ) / ( epsilon * epsilon * ept);
            cout << "Refind KPT value for R: " << R << endl;
            BuildHypergraphR(R);
            cout << "Number of RR sets: " << hyperGT.size() << endl;
        }
        double logcnk(int n, int k){
            double ans=0;
            for(int i=n-k+1; i<=n; i++){
                ans+=log(i);
            }
            for(int i=1; i<=k; i++){
                ans-=log(i);
            }
            return ans;
        }
        void NodeSelection(double epsilon, double opt){
            Timer t(3, "step3");
            ASSERT(opt > 0);
            //cout << "OPT: " << opt << endl;
         //int64 R = (8+2 * epsilon) * ( log(n_prime) + log(2) +  n_prime * logcnk(n_prime, k) ) / ( epsilon * epsilon * opt);
           //  cout << "Node Selection value for R: " << R << endl;
           //  BuildHypergraphR(R);
           //  cout << "Number of RR sets: " << hyperGT.size() << endl;
        // BuildSeedSet();
        }
        void EstimateOPT(double epsilon){
            Timer t(100,"EstimateOPT");

            // KPT estimation
            double kpt_star;
            kpt_star=KptEstimation();

            // Refine KPT
            double eps_prime;
            eps_prime=5*pow(sqr(epsilon)/(k+1), 1.0/3.0);
            RefindKPT(eps_prime, kpt_star);


			double kpt = BuildSeedSet();;
            kpt/=1+eps_prime;
            double kpt_plus = max(kpt, kpt_star);

            cout << "Estimated Influence: " << kpt_plus << endl;
            
            //Node Selection
            NodeSelection(epsilon, kpt_plus);
            disp_mem_usage("");
        }

};

