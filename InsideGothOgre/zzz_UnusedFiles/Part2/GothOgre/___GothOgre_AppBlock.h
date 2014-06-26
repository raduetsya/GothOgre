#ifndef GOTHOGRE_APP_BLOCK_H
#define GOTHOGRE_APP_BLOCK_H


namespace GothOgre
{
	class AppBlock
	{
	public:
		AppBlock() {}
		virtual ~AppBlock() {}
		virtual void initialise() {}
		virtual void shutdown() {}
		virtual void postinitialise() {}
		virtual void preshutdown() {}
	};

} // namespace GothOgre

#endif // GOTHOGRE_APP_BLOCK_H