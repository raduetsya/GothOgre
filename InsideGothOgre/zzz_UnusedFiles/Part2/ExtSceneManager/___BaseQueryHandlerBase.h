#ifndef QUERY_HANDLER_BASE_H
#define QUERY_HANDLER_BASE_H


namespace GothOgre
{
	class SceneManagerExImpl;


	//-------------------------------------------------------------------------
	class QueryHandlerBase
	{
	public:
		QueryHandlerBase(SceneManagerExImpl* _sm);
		virtual ~QueryHandlerBase();

		/** Creates ray query. GothOgre provides its own "RaySceneQuery" class. */
		RaySceneQueryEx* createRayQueryEx();
		
	protected:
		SceneManagerExImpl*    mSceneManager;
	};

} // namespace GothOgre

#endif // QUERY_HANDLER_BASE_H