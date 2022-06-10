#include<iostream>
#include<algorithm>
#include<cstdlib>
#include<fstream>
#include "Eigen/Dense"
using namespace std;
using namespace Eigen;

void featurenormalize(MatrixXd &X)
{
	//����ÿһά�Ⱦ�ֵ
	MatrixXd meanval = X.colwise().mean();
	RowVectorXd meanvecRow = meanval;
	//������ֵ��Ϊ0
	X.rowwise() -= meanvecRow;
}
void computeCov(MatrixXd &X, MatrixXd &C)
{
	//����Э�������C = XTX / n-1;
	C = X.adjoint() * X;
	C = C.array() /(X.rows() - 1);
}
void computeEig(MatrixXd &C, MatrixXd &vec, MatrixXd &val)
{
	//��������ֵ������������ʹ��selfadjont���ն��������㷨ȥ���㣬�����ò�����vec��val������������
	SelfAdjointEigenSolver<MatrixXd> eig(C);

	vec = eig.eigenvectors();
	val = eig.eigenvalues();
}
int computeDim(MatrixXd &val)
{
	int dim;
	double sum = 0;
	for (int i = val.rows() - 1; i >= 0; --i)
	{
		sum += val(i, 0);
		dim = i;

		if (sum / val.sum() >= 0.95)
			break;
	}
	return val.rows() - dim;
}
int main()
{
	ifstream fin("d:\\test1.txt");
	ofstream fout("d:\\test2.txt");
	const int m = 10000, n = 128;
	MatrixXd X(10000, 128), C(128, 128);
	MatrixXd vec, val;

	//��ȡ����
	double in[200];
	for (int i = 0; i < m; ++i)
	{
		for (int j = 0; j < n; ++j)
			fin >> in[j];
		for (int j = 1; j <= n; ++j)
			X(i, j - 1) = in[j - 1];
	}
	//pca

	//���ֵ��
	featurenormalize(X);
	//����Э����
	computeCov(X, C);
	//��������ֵ����������
	computeEig(C, vec, val);
	//������ʧ�ʣ�ȷ������ά��
	int dim = computeDim(val);
	//������
	MatrixXd res = X * vec.rightCols(dim);
	//������
	fout << "the result is " << res.rows() << "x" << res.cols() << " after pca algorithm." << endl;
	fout << res;
	system("pause");
	return 0;
}