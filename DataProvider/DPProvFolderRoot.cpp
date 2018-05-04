#include "DPProvFolderRoot.hpp"


CDPProvFolderRoot::CDPProvFolderRoot()
{
}


CDPProvFolderRoot::~CDPProvFolderRoot()
{
}

bool CDPProvFolderRoot::Initialize()
{
	return false;
}

std::shared_ptr<CDPProvFolderProduct>  CDPProvFolderRoot::GetFolderProduct(std::string strProductName)
{
	return nullptr;
}

