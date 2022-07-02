#include<iostream>
#include<random>
#include<vector>
#include<iterator>
#include<numeric>
#include<algorithm>
#include<memory>
#include<ctime>
using namespace std;
using Gene = vector<char>;
class GA {
public:
	GA(vector<int> &&weight, int n, int m): limit(m), total(n) {
		gene_size = weight.size();
		/*for (auto&& it : weight) {
			this->weight.push_back(move(it));
		}*/
		this->weight = weight;
		//v_gene = vector<Gene>(gene_cnt, Gene(gene_size, false));
		dis = m;
		times = 0;
	}
	double mutation_pro = 0.1;
	int gene_cnt = 20;
	int cross_cnt = 10;
	bool do_work = true;
	Gene ans;
	vector<int> weight;
	void start();
private:
	vector<Gene> v_gene;
	int gene_size;
	int total;
	int limit;
	int times;
	int dis;
	vector<int> v_fit;
	void init();
	void select();
	void cross();
	void mutation();
	void swap_gene(Gene &parent1, Gene &parent2, int l_interval, int r_interval);
	bool check(int parent1, int parent2, int l_interval, int r_interval);
	int cal(Gene &gene);
};
int feasible_fun(Gene& gene, vector<int>& weight, int m = 0) {
	int score = 0;
	for (int i = 0; i < gene.size(); i++) {
		if(gene[i])
			score += weight[i];
	}
	score = m - score;
	return score;
}
int random_t(int l ,int r) {
	return l + rand() % (r - l + 1);
}

void GA::start()
{
	this->init();
	while (do_work) {
		this->select();
		if (times > 6000) {
			break;
		}
		if (do_work) {
			this->cross();
			this->mutation();
		}
	}
}

void GA::init()
{
	v_gene = vector<Gene>(gene_cnt, Gene(gene_size, false));
	int t = total / limit + 1;
	for (int i = 0; i < gene_cnt; i++) {
		for (int j = 0; j < gene_size; j++) {
			if (random_t(0, t) == 0) {
				v_gene[i][j] = true;
			}
		}
	}
}

void GA::select()
{
	for (auto &&gene : this->v_gene) {
		v_fit.push_back(feasible_fun(gene, this->weight, limit));
		if (v_fit.back() >= 0 && v_fit.back() < this->dis) {
			dis = v_fit.back();
			ans = gene;
		}
		if (v_fit.back() >= 0) {
			v_fit.back() = this->total + limit - v_fit.back();
		}
		else {
			v_fit.back() += this->total;
		}
		if (dis == 0) {
			this->do_work = false;
			return;
		}
	}
	times++;
	int res = accumulate(v_fit.begin(), v_fit.end(), 0);
	vector<int> expect;
	for (int i = 0; i < gene_cnt; i++) {
		expect.push_back(1.0 * v_fit[i] / res * gene_cnt);
	}
	int cnt = accumulate(expect.begin(), expect.end(), 0);
	while (cnt < gene_cnt) {
		expect[random_t(0, gene_cnt - 1)]++, cnt++;
	};
	vector<int> tmp;
	for (int i = 0; i < expect.size(); i++) {
		for (int j = 0; j < expect[i]; j++) {
			tmp.emplace_back(v_fit[i]);
		}
	}
	v_fit = tmp;

}

void GA::cross()
{
	for (int i = 0; i < cross_cnt; i++){
		int parent1 = random_t(0, gene_cnt - 1);
		int parent2 = random_t(0, gene_cnt - 1);
		while (parent1 == parent2) {
			parent2 = random_t(0, gene_cnt - 1);
		}
		int l_interval = random_t(1, gene_size - 1);
		int r_interval = random_t(1, gene_size - 1);
		while (l_interval == r_interval) {
			r_interval = random_t(1, gene_size - 1);
		}
		if (r_interval < l_interval) {
			swap(l_interval, r_interval);
		}
		if (!check(parent1, parent2, l_interval, r_interval)) {
			swap_gene(this->v_gene[parent1], this->v_gene[parent2], l_interval, r_interval);
		}
	}
}

void GA::mutation()
{
	int mutation_times = mutation_pro * gene_cnt * gene_size;
	for (int i = 0; i < mutation_times; i++) {
		int v_gene_index = random_t(0, gene_cnt - 1), gene_index = random_t(0, gene_size-1);
		Gene tmp_g = v_gene[v_gene_index];
		if (tmp_g[gene_index]) {
			tmp_g[gene_index] = false;
		}
		else {
			tmp_g[gene_index] = true;
		}
		if (cal(tmp_g) < v_fit[v_gene_index]) {
			continue;
		}
		else {
			v_gene[v_gene_index][gene_index] = tmp_g[gene_index];
			v_fit[v_gene_index] = cal(v_gene[v_gene_index]);
		}
		

	}
}

void GA::swap_gene(Gene &parent1, Gene &parent2, int l_interval, int r_interval)
{
	for (int i = l_interval; i <= r_interval; i++) {
		swap(parent1[i], parent2[i]);
	}
}

bool GA::check(int parent1, int parent2, int l_interval, int r_interval)
{
	Gene p1 = v_gene[parent1], p2 = v_gene[parent2];
	swap_gene(p1, p2, l_interval, r_interval);
	int v_p1 = cal(p1), v_p2 = cal(p2);
	return v_p1 <= v_fit[parent1] && v_p2 <= v_fit[parent2];
}

int GA::cal(Gene &gene)
{
	int score = feasible_fun(gene, weight, limit);
	if (score >= 0) {
		return total + limit - score;
	}
	return total + score;
}

int main() {
	srand(time(0));
	int n, m;
	ios::sync_with_stdio(false);
	cin.tie(nullptr);
	cin >> n >> m;
	vector<int> v(n);
	copy_n(istream_iterator<int>(cin), n, v.begin());
	int sum = accumulate(v.begin(), v.end(), 0);
	if (sum < m) {
		cout << "No Solution!\n";
	}
	else {
		auto ga = make_shared<GA>(move(v), sum, m);
		ga->start();
		if (ga->do_work) {
			cout << "No Solution!\n";
		}
		else {
			for (int i = 0; i < n; i++) {
				if (ga->ans[i]) {
					cout << ga->weight[i] << ' ';
				}
			}
		}
	}
	return 0;
}

