namespace Tlahtolli
{
    internal class SoundStackFactory : IStackFactory
    {

        private static SoundStackFactory instance;

        /// <summary>
        /// Initializes a new instance of the <see cref="IStackFactory"/> class.
        /// </summary>
        /// <remarks>
        /// The constructor is declared protected as its only meant to be instantiated by <see cref="Instance"/>.
        /// </remarks>
        protected SoundStackFactory()
        {
        }

        /// <summary>
        /// Gets or sets the default <see cref="SoundStackFactory"/> instance used by <see cref="SoundStack"/>.
        /// </summary>
        public static SoundStackFactory Instance
        {
            get { return instance ?? (instance = new SoundStackFactory()); }
            set { instance = value; }
        }

        public override IStack GetStack()
        {
            return new SoundStack();
        }
    }
}
